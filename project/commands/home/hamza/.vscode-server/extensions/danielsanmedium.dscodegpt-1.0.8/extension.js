//para manipular vscode
const vscode = require('vscode');

const API_KEY = "API_KEY";
let languageId;

//función asincrona para enviar la pregunta a Openai
async function getCodeGPTOutput(text, type, context) {
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

	vscode.languages.setTextDocumentLanguage(outputDocument, languageId);

	//limpiamos el texto que ingresó el usuario
	const cleanPromptText = text.split("\r\n").join("\n");
	let responseText = "";
	try {
		const apiKeyType = vscode.workspace.getConfiguration().get('CodeGPT.apiKey');
		if(apiKeyType == 'OpenAI')
		{
			responseText = await getOpenAI(cleanPromptText, type, context);
		}
		
	}
	catch (error) {
		console.log(error);
	}

	// nos llega correctamente la respuesta y la agregamos al editor
	console.log("respuesta ->", responseText)



	//la cargamos en el editor 🙌
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
		let comment_character = '';
		const editor = vscode.window.activeTextEditor;
		const document = editor.document;
		languageId = editor.document.languageId;

		switch (languageId) {
			case 'javascript':
				comment_character = '//';
				break;
			case 'typescript':
				comment_character = '//';
				break;
			case 'python':
				comment_character = '#';
				break;
			case 'php':
				comment_character = '//';
				break;
			case 'c++':
				comment_character = '//';
				break;
			case 'java':
				comment_character = '//';
				break;
			case 'ruby':
				comment_character = '#';
				break;
			case 'golang':
				comment_character = '//';
				break;
			case 'html':
				comment_character = '<!--';
				break;
			case 'css':
				comment_character = '/*';
				break;
			case 'C#':
				comment_character = '//';
				break;
			case 'dart':
				comment_character = '//';
				break;
			case 'vue':
				comment_character = '//';
				break;
			default:
				vscode.window.showErrorMessage(
					"This language is not supported",
				)
				return;
		}

		if (!editor) {
			vscode.window.showInformationMessage('Open an editor.');
			return;
		}
		if (languageId == '') {
			vscode.window.showErrorMessage(
				"This language is not supported",
			)
			return;
		}

		let selection;
		const cursorPosition = editor.selection.active;
		selection = new vscode.Selection(cursorPosition.line, 0, cursorPosition.line, cursorPosition.character);
		//console.log(document.getText(selection))

		let comment = document.getText(selection);

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
		const final_comment = comment.replaceAll(comment_character, languageId + ': ');
		console.log(final_comment);

		getCodeGPTOutput(final_comment, 'getCodeGPT', context);
	});

	let askCodeGPT = vscode.commands.registerCommand('codegpt.askCodeGPT', async () => {

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
			getCodeGPTOutput(text_final, 'askCodeGPT', context);
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
			getCodeGPTOutput(selectedText, 'explainCodeGPT', context);
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
			getCodeGPTOutput(selectedText, 'refactorCodeGPT', context);
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
			getCodeGPTOutput(selectedText, 'documentCodeGPT', context);
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
			getCodeGPTOutput(selectedText, 'findProblemsCodeGPT', context);
		}
	});

	//suscribimos los eventos
	context.subscriptions.push(askCodeGPT, commandExplainCodeGPT, commandRefactorCodeGPT,
		commandDocumentCodeGPT, commandFindProblemsCodeGPT, getCode, setApiKeyCodeGPT, removeApiKeyCodeGPT);
}

// This method is called when your extension is deactivated
function deactivate() { }

module.exports = {
	activate,
	deactivate
}
