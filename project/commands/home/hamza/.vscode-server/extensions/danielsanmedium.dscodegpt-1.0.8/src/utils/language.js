// function to detect a language
let comment_character = '';

const detectLanguage = (languajeId) => {
    switch (languajeId) {
        case 'javascript':
            comment_character = '//';
            break;
        case 'typescriptreact':
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
        case '':
            comment_character = false;
            break;
        default:
            comment_character = false;
            return;
    }

    return comment_character;
}

module.exports = { detectLanguage }