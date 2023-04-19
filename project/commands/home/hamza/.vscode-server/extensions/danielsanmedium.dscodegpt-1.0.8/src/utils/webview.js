const vscode = require('vscode');

// Función para crear y mostrar el WebView
const createWebViewPanel = (type, response, context) => {

    // Crear un nuevo WebView
	const panel = vscode.window.createWebviewPanel(
		'codegpt-panel-view', // Identificador único
		'Chat Code GPT', // Título
		vscode.ViewColumn.Beside, // Columna donde se mostrará
		{
			enableScripts: true, // Habilitar scripts

            localResourceRoots:
            [
                vscode.Uri.joinPath(context.extensionUri, "media")
            ]
		}
	);

    if (typeof response === 'undefined')
    {
        response = 'Oops! Something happened, check the extension settings';
    }

    let clear_response = response.trim().split("\r\n").join("\n");
    let clean_response = JSON.stringify(clear_response, null, 2);
    console.log(clean_response);

    const webview = panel.webview;

    //JS and CSS Files
    const styleMain = webview.asWebviewUri( 
            vscode.Uri.joinPath(context.extensionUri, 'media', 'main.css')
    );
    const highlightMinJs = webview.asWebviewUri( 
        vscode.Uri.joinPath(context.extensionUri, 'media', 'highlight.min.js')
    );
    const showdownJs = webview.asWebviewUri( 
        vscode.Uri.joinPath(context.extensionUri, 'media', 'showdown.min.js')
    );

    // Use a nonce to only allow specific scripts to be run
	const nonce = getNonce();

    const html = `
    <!doctype html>
    <html>
    <head>
        <meta charset="UTF-8">

        <meta http-equiv="Content-Security-Policy" content="img-src https: data:; style-src 'unsafe-inline' ${webview.cspSource}; script-src 'nonce-${nonce}';">

        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="${styleMain}">
    </head>

    <body class="background: black">
    <main class="p-3 h-100 w-100 bg-secondary bg-opacity-10">
        <h1 class="h5 mb-3 text-center"><b>Code GPT</b></h1>
        <div class="response rounded-4 bg-secondary bg-opacity-10 p-3" id="typing">
            <h3 class="h3 mb-3">${type}</h3>
            <div id="snippet">
                <pre>
                    <code id="resp" class="hljs typed-out">
                    </code>
                </pre>
            </div>
            <button class="btn btn-secundary btn-sm" id="btn_copy">Copy</button>
        </div>
    </main>
    <footer class="position-fixed d-flex bg-secondary bg-opacity-10">
        <input type="text" class="form-control me-3" placeholder="Ask CodeGPT..">
        <button class="btn btn-primary">
        <svg class="svg-icon" style="width: 1em; height: 1em;vertical-align: middle;fill: currentColor;overflow: hidden;" viewBox="0 0 1024 1024" version="1.1" xmlns="https://www.w3.org/2000/svg"><path d="M729.173333 469.333333L157.845333 226.496 243.52 469.333333h485.674667z m0 85.333334H243.541333L157.824 797.504 729.173333 554.666667zM45.12 163.541333c-12.352-34.986667 22.762667-67.989333 56.917333-53.482666l853.333334 362.666666c34.645333 14.72 34.645333 63.829333 0 78.549334l-853.333334 362.666666c-34.133333 14.506667-69.269333-18.474667-56.917333-53.482666L168.085333 512 45.098667 163.541333z" fill="" /></svg>
        </button>
    </footer>
    <script nonce="${nonce}" src="${highlightMinJs}"></script>
    <script nonce="${nonce}">hljs.highlightAll();</script>
    <script nonce="${nonce}" src="${showdownJs}"></script>
    <script nonce="${nonce}">
            var converter = new showdown.Converter();
            var text      = ${clean_response};
            var html      = converter.makeHtml(text);
            document.getElementById('resp').innerHTML = html;

            document.getElementById('btn_copy').onclick = function(){
                containerid = 'resp';
                if (document.selection) {
                    var range = document.body.createTextRange();
                    range.moveToElementText(document.getElementById(containerid));
                    range.select().createTextRange();
                    document.execCommand("copy");
                } else if (window.getSelection) {
                    var range = document.createRange();
                    range.selectNode(document.getElementById(containerid));
                    window.getSelection().addRange(range);
                    document.execCommand("copy");
                }
            }  

    </script>
    </body>
</html>`;
    panel.webview.html = html;
}

function getNonce() {
	let text = '';
	const possible = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
	for (let i = 0; i < 32; i++) {
		text += possible.charAt(Math.floor(Math.random() * possible.length));
	}
	return text;
}

// Registrar el comando para mostrar el WebView
module.exports = { createWebViewPanel }