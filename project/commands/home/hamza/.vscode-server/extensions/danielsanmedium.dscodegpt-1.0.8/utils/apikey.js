


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