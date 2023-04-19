const { Configuration, OpenAIApi } = require('openai');

const createOpenAiClient = (apiKey) => {
    const configuration = new Configuration({
        apiKey: apiKey
    });
    const openai = new OpenAIApi(configuration);

    return openai;
}

module.exports = createOpenAiClient;