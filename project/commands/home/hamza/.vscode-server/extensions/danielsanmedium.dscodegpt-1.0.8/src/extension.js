const vscode = require('vscode');
const webview = require('./utils/webview.js');
const language = require('./utils/language.js');
const prompts = require('./utils/prompts.js');
//const openai_client = require('./clients/openai.js');

const { Configuration, OpenAIApi } = require("openai");

// OPENAI API_KEY
const API_KEY = "API_KEY";

//creamos openai con los parametros del setting
async function getOpenAI(cleanPromptText, promptType, context) {
	//Obtenemos los datos para llamar a la api de openai desde settings
	//const apiKey = vscode.workspace.getConfiguration().get('CodeGPT.apiKey');
	const apiKey = await context.secrets.get(API_KEY);

	if (!apiKey) {
		setApiKey(context);
		vscode.window.showWarningMessage('Enter your API KEY to save it securely.');
		return;
	}
	
	let model = vscode.workspace.getConfiguration().get('CodeGPT.model');
	let temperature = vscode.workspace.getConfiguration().get('CodeGPT.temperature');
	let maxTokens = vscode.workspace.getConfiguration().get('CodeGPT.maxTokens');
	let language = vscode.workspace.getConfiguration().get('CodeGPT.query.language');

	//si no viene el modelo ocupamos text-davinci-003
	if (model == "") {
		model = 'text-davinci-003';
	}

	//si no viene la temperatura ocupamos 0.3
	if (temperature == "") {
		temperature = 0.3;
	}

	//si no vienen los tokens ocupamos 250
	if (maxTokens == "") {
		maxTokens = 250;
	}

	//construimos la configuración de openai con la api key
	const configuration = new Configuration({
		apiKey: apiKey,
	});

	//creamos el objeto openai para hacer la consulta a la api
	const openai = new OpenAIApi(configuration);

	//le damos un one shot prompt al texto
	let one_shot_prompt = prompts.getCommandPrompt(cleanPromptText, promptType, language);

	// Inicializa el progreso
	const progressOptions = {
		location: vscode.ProgressLocation.Notification,
		title: "CodeGPT",
		cancellable: true
	};

	let respuesta;

	await vscode.window.withProgress(progressOptions, async (progress, token) => {

		// Update the progress bar
		progress.report({ message: "Formatting text" });

		// if the progress is canceled
		if (token.isCancellationRequested) {
			return;
		}

		// Update the progress bar
		progress.report({ message: "I am thinking..." });
		
		try{
			const completion = await openai.createCompletion({
				model: model,
				prompt: one_shot_prompt,
				temperature: temperature,
				max_tokens: maxTokens,
				top_p: 1.0,
				frequency_penalty: 0.5,
				presence_penalty: 0.0,
				stop: ["END"],
			});
			
			//OPENAI completion
			respuesta = completion.data.choices[0].text;

			if(respuesta == '')
			{
				respuesta = 'The API could not process the query, try selecting the code and using Ask CodeGPT to write your own query';
			}

			if(completion.data.usage.total_tokens && completion.data.usage.total_tokens >= maxTokens) 
			{	
				vscode.window.showWarningMessage(`Ops! Incomplete Completion.
				The request requires ${completion.data.usage.total_tokens} tokens and you only have ${maxTokens}. Add more tokens in your CodeGPT Settings.`);
			}

		}
		catch(error)
		{
			respuesta = 'Ops! Account problems. More information in the VSCode notification error.'
			console.log(error);
			vscode.window.showErrorMessage(
				error.response.data.error.message,
			)
		}
		
		progress.report({ increment: 100, message: "" });
	}).then(undefined, err => {
		console.error(err);
	});
	
	return respuesta;
}

// asynchronous function to send the query to the provider
async function getCodeGPTOutput(text, type, context, languageId) {
	const outputDocument = await vscode.workspace.openTextDocument({
		content: "Loading...",
		language: "markdown",
	})
	const outputDocumentEditor = await vscode.window.showTextDocument(
		outputDocument,
		{
			viewColumn: vscode.ViewColumn.Beside,
			preserveFocus: true,
			preview: true
		},
	)
	
	if(languageId != null)
	{
		vscode.languages.setTextDocumentLanguage(outputDocument, languageId);
	}

	//limpiamos el texto que ingresó el usuario
	const cleanPromptText = text.split("\r\n").join("\n");
	let responseText = "";
	try {
		responseText = await getOpenAI(cleanPromptText, type, context);
	}
	catch (error) {
		console.log(error);
	}

	//ShowPanel(type, responseText, context);

	//la cargamos en el editor
	outputDocumentEditor.edit(editBuilder => {
		editBuilder.replace(
			new vscode.Range(
				new vscode.Position(0, 0),
				new vscode.Position(99999999999999, 0),
			),
			`${responseText}`,
		)
	});
}

async function setApiKey(context){
	// Mostramos un cuadro de diálogo al usuario para que ingrese la token
	const api_key = await vscode.window.showInputBox({
		title: "Enter your API KEY",
		password: true,
		placeHolder: 'sk-**********************************',
		ignoreFocusOut: true
	});

	// Si el usuario canceló el cuadro de diálogo, no hacemos nada
	if (!api_key) {
		vscode.window.showWarningMessage('Empty value');
		return;
	}

	// Storing a secret
	await context.secrets.store(API_KEY, api_key);

	// Mostramos un mensaje al usuario para confirmar que la token se ha guardado de forma segura
	vscode.window.showInformationMessage('API KEY saved');
}

// Init Webview
async function ShowPanel(type, response, context){
	// Set the HTML and JavaScript content of the WebView
	webview.createWebViewPanel(type, response, context);
}

/**
 * @param {vscode.ExtensionContext} context
 */
async function activate(context) {

	let setApiKeyCodeGPT = vscode.commands.registerCommand('codegpt.setApiKeyCodeGPT', async() => {
		
		setApiKey(context);

	});

	let removeApiKeyCodeGPT = vscode.commands.registerCommand('codegpt.removeApiKeyCodeGPT', async () => {
		
		await context.secrets.delete(API_KEY);
		vscode.window.showWarningMessage('Your API KEY was removed');
	});

	let getCode = vscode.commands.registerCommand('codegpt.getCode', async () => {
		const editor = vscode.window.activeTextEditor;
		const document = editor.document;
		const languageId = editor.document.languageId;

		const comment_character = language.detectLanguage(languageId);
		if(comment_character == false)
		{
			vscode.window.showErrorMessage(
                "This language is not supported",
            );
			return;
		}

		if (!editor) {
			vscode.window.showInformationMessage('Open an editor.');
			return;
		}

		let selection;
		const cursorPosition = editor.selection.active;
		selection = new vscode.Selection(cursorPosition.line, 0, cursorPosition.line, cursorPosition.character);
		//console.log(document.getText(selection))

		let comment = document.getText(selection);

		let one_shot_prompt = languageId;
		if (comment == '') {
			vscode.window.showErrorMessage(
				"You must to create a comment",
			)
			return;
		}
		// el caracter existe
		const exist = comment.indexOf(comment_character);
		if (exist == -1) {
			vscode.window.showErrorMessage(
				"You must to create a comment",
			)
			return;
		}
		const final_comment = comment.replaceAll(comment_character, one_shot_prompt + ': ');
		console.log(final_comment);

		getCodeGPTOutput(final_comment, 'getCodeGPT', context, languageId);
	});

	let askCodeGPT = vscode.commands.registerCommand('codegpt.askCodeGPT', async () => {
		
		// validate to have an editor tab open
		if(vscode.window.activeTextEditor == undefined)
		{
			vscode.window.showWarningMessage(
				"To get started, you must first have an editor tab open"
			)
			return;
		}

		//abrimos un input de texto para preguntar
		const text = await vscode.window.showInputBox({
			title: "Ask CodeGPT",
			prompt: "Enter question",
			placeHolder: "Question",
		})
		if (text) {
			let text_final = '';
			const selection = vscode.window.activeTextEditor.selection;
			const selectedText = vscode.window.activeTextEditor.document.getText(selection);
			if (selectedText != '') {
				let language = vscode.workspace.getConfiguration().get('CodeGPT.query.language');
				let prefijo = '';
				if (language == 'Spanish') {
					prefijo = ', con respecto al siguiente código: ';
				}
				else {
					prefijo = ', according to the following code: ';
				}
				text_final = text + prefijo + selectedText.split("\r\n").join("\n");
			}
			else {
				text_final = text;
			}
			//enviamos la pregunta a una función asyncrona que abrirá
			//otro editor con la respuesta
			getCodeGPTOutput(text_final, 'askCodeGPT', context, null);
		} else {
			vscode.window.showErrorMessage(
				"Empty text!",
			)
		}
	});

	let commandExplainCodeGPT = vscode.commands.registerCommand('codegpt.explainCodeGPT', async () => {

		const selection = vscode.window.activeTextEditor.selection;
		const selectedText = vscode.window.activeTextEditor.document.getText(selection);
		if (selectedText == '') {
			vscode.window.showErrorMessage(
				"No text selected!",
			)
		} else {
			getCodeGPTOutput(selectedText, 'explainCodeGPT', context, null);
		}
	});

	let commandRefactorCodeGPT = vscode.commands.registerCommand('codegpt.refactorCodeGPT', async () => {

		const selection = vscode.window.activeTextEditor.selection;
		const selectedText = vscode.window.activeTextEditor.document.getText(selection);
		if (selectedText == '') {
			vscode.window.showErrorMessage(
				"No text selected!",
			)
		} else {
			getCodeGPTOutput(selectedText, 'refactorCodeGPT', context, null);
		}
	});

	let commandDocumentCodeGPT = vscode.commands.registerCommand('codegpt.documentCodeGPT', async () => {

		const selection = vscode.window.activeTextEditor.selection;
		const selectedText = vscode.window.activeTextEditor.document.getText(selection);
		if (selectedText == '') {
			vscode.window.showErrorMessage(
				"No text selected!",
			)
		} else {
			getCodeGPTOutput(selectedText, 'documentCodeGPT', context, null);
		}
	});

	let commandFindProblemsCodeGPT = vscode.commands.registerCommand('codegpt.findProblemsCodeGPT', async () => {

		const selection = vscode.window.activeTextEditor.selection;
		const selectedText = vscode.window.activeTextEditor.document.getText(selection);
		if (selectedText == '') {
			vscode.window.showErrorMessage(
				"No text selected!",
			)
		} else {
			getCodeGPTOutput(selectedText, 'findProblemsCodeGPT', context, null);
		}
	});

	let commandUnitTestCodeGPT = vscode.commands.registerCommand('codegpt.unitTestCodeGPT', async () => {

		const selection = vscode.window.activeTextEditor.selection;
		const selectedText = vscode.window.activeTextEditor.document.getText(selection);
		if (selectedText == '') {
			vscode.window.showErrorMessage(
				"No text selected!",
			)
		} else {
			getCodeGPTOutput(selectedText, 'unitTestCodeGPT', context, null);
		}
	});

	//subscribed events
	context.subscriptions.push(askCodeGPT, 
			commandExplainCodeGPT, 
			commandRefactorCodeGPT,
			commandDocumentCodeGPT, 
			commandFindProblemsCodeGPT, 
			getCode, 
			setApiKeyCodeGPT, 
			removeApiKeyCodeGPT,
			commandUnitTestCodeGPT);
}

// This method is called when your extension is deactivated
function deactivate() { }

module.exports = {
	activate,
	deactivate
}
