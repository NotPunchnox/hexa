import {ChatOllama} from "@langchain/community/chat_models/ollama";
import {StringOutputParser} from "@langchain/core/output_parsers";
import {PromptTemplate} from "@langchain/core/prompts";
import {RunnableSequence} from "@langchain/core/runnables";
import {formatDocumentsAsString} from "langchain/util/document";
import embedding from "../controller/embedding.js";
import config from "../../examples/client.js";
import configConst from "../../config.json"assert {    type: "json"};
import tts from "../controller/tts.js";
import {SerialPort} from "serialport";
import {ReadlineParser} from "serialport";

var ChatHistory = []
var parser,port,model,model_name;

if (configConst.serial) {
    // Configurez le port série
    port = new SerialPort({
        path: configConst.COM_SERIAL,
        baudRate: 115200
    });

    parser = port.pipe(new ReadlineParser({
        delimiter: '\n'
    }));
}

export default async (prompt, l = 3, modelSelected = config.LLM_MODEL) => {
    let Timer = Date.now()

    if (ChatHistory.length > 5) ChatHistory = ChatHistory.slice(2, ChatHistory.length)
    ChatHistory.push(`user: ${prompt}`)

    if(model_name !== modelSelected) {
        model = new ChatOllama({
            baseUrl: config.OLLAMA_API_URL,
            model: modelSelected,
            format: config.format !== "json" ? null : config.format
        });
        model_name = modelSelected
    }

    try {
        let {
            resultActions,
            resultConversations,
            resultGlobal
        } = await embedding.search(prompt, l);
        resultActions = formatDocumentsAsString(resultActions);
        resultConversations = formatDocumentsAsString(resultConversations);
        resultGlobal = formatDocumentsAsString(resultGlobal);
        // console.log(resultConversations, resultGlobal, resultActions)
        const textTemplate = 
        String(`Répondez à la question suivante en format JSON. Soyez clair et concis. Si des actions sont requises, utilisez-les, sinon laissez le tableau d'actions vide.
 
${resultGlobal && resultGlobal.length > 0 ? "context: {context};" : "{context}"}
${ChatHistory && ChatHistory.length > 0 ? "history: {ChatHistory};" : "{ChatHistory}"}
${resultConversations && resultConversations.length > 0 ? "memory: {conversation};" : "{conversation}"}
actions: {syntaxAction};
QUESTION: {question};

Réponse:`);
        
        console.log(`Répondez à la question suivante en format JSON. Soyez clair et concis. Si des actions sont requises, utilisez-les, sinon laissez le tableau d'actions vide.
 
${resultGlobal && resultGlobal.length > 0 ? "context: "+resultGlobal+";" : ""}
${ChatHistory && ChatHistory.length > 1 ? "history: "+ChatHistory+";" : "{ChatHistory}"}
${resultConversations && resultConversations.length > 0 ? "memory: "+resultConversations+";" : "{conversation}"}
actions: ${resultActions};
QUESTION: ${prompt};

Réponse:`);
        
        const PROMPT_TEMPLATE = PromptTemplate.fromTemplate(textTemplate);
        

        const chain = RunnableSequence.from([
            async (input) => ({
                    context: resultGlobal,
                    conversation: resultConversations,
                    ChatHistory: JSON.stringify(ChatHistory),
                    syntaxAction: resultActions,
                    question: input.question
                }),
                PROMPT_TEMPLATE,
                model,

                new StringOutputParser()
        ]);

        if (config.stream) {
            const finalResult = await chain.stream({
                context: resultGlobal,
                conversation: resultConversations,
                syntaxAction: resultActions,
                ChatHistory: ChatHistory,
                question: prompt
            });

            process.stdout.write('\n\x1b[1mRéponse:\x1b[0m\x1b[36m ');
            let chunks = []
            for await (const chunk of finalResult) process.stdout.write(chunk), chunks.push(chunk);
            process.stdout.write('\x1b[0m\n');

            ChatHistory.push(`AI: ${chunks.join('').replace(/\n|\r/g, '')}`)

            console.log(`Temps de génération: ${((Date.now()-Timer)/1000)}s`)

            if (config.serial === true && JSON.parse(chunks.join('')) && JSON.parse(chunks.join('')).actions) {
                    JSON.parse(chunks.join('')).actions.forEach(a => {
                        port.write(a + '\n', (err) => {
                            if (err) return console.error('Serial Error:', err)

                            parser.once('data', (data) => {
                                console.log("Info Hardware:", data.trim());
                            });
                        });
                })
            }

            const res = JSON.parse(chunks.join('').replace(/\n|\r/g, ''))

            if (config.tts) {
                try {
                    console.log(res)
                    tts(res?.message) //Text to speech
                } catch (err) {
                    console.log('impossible de formatter le contenu de la réponse')
                }
            }

            return {result: res, resultConversations}
        } else {
            const result = await chain.invoke({
                context: resultGlobal,
                conversation: resultConversations,
                syntaxAction: resultActions,
                ChatHistory: ChatHistory,
                question: prompt
            });
            return {result, resultConversations}
        }
    } catch (error) {
        console.error('Error:', error);
    }
}

export { ChatHistory }