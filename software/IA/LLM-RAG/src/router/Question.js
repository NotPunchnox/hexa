import {ChatOllama} from "@langchain/ollama";
import {StringOutputParser} from "@langchain/core/output_parsers";
import {PromptTemplate} from "@langchain/core/prompts";
import {RunnableSequence} from "@langchain/core/runnables";
import {formatDocumentsAsString} from "langchain/util/document";
import embedding from "../controller/embedding.js";
// import config from "../../examples/client.js";
import configConst from "../../config.json"assert {    type: "json"};
import tts from "../controller/tts.js";
import {SerialPort, ReadlineParser} from "serialport";
import PORT from "./serialPort.js";
import axios from "axios";

var ChatHistory = []
var parser,port,model,model_name;

if (configConst.serial) {
    console.log(await PORT)
    // Configurez le port série
    port = new SerialPort({
        path: await PORT,
        baudRate: 115200
    });

    parser = port.pipe(new ReadlineParser({
        delimiter: '\n'
    }));
}

export default async (prompt, l = 3, modelSelected = configConst.LLM_MODEL) => {
    let Timer = Date.now()

    if (ChatHistory.length > 5) ChatHistory = ChatHistory.slice(2, ChatHistory.length)
    ChatHistory.push(`user: ${prompt}`)

    if(model_name !== modelSelected) {
        model = new ChatOllama({
            baseUrl: configConst.OLLAMA_API_URL,
            model: modelSelected,
            format: configConst.format !== "json" ? null : configConst.format
        });
        model_name = modelSelected
    }

    try {
        let {
            resultActions,
            resultConversations,
            resultGlobal
        } = await embedding.search(prompt, l);
        resultActions = formatDocumentsAsString(resultActions).replace(/\n/g, '');
        resultConversations = formatDocumentsAsString(resultConversations).replace(/\n/g, '');
        resultGlobal = formatDocumentsAsString(resultGlobal).replace(/\n/g, '');

        let response_camera;

        try {
            response_camera = await axios.get(configConst.VISION_API_URL + '/api/results');
            const data = response_camera.data
            console.log(data)
            // const data = JSON.parse(response_camera.data)
            if(data) {
                response_camera = `faces: ${data.faces.join(',')}. objects: ${JSON.stringify(data.objects)}`
            }
        } catch (err) {
            console.error(err)
            response_camera = "Camera not found"
        }
        // response_camera = "Camera not found"

        // console.log(resultConversations, resultGlobal, resultActions)
        const textTemplate = String(`Répondez à la question suivante en format JSON. Soyez clair et concis. Si des actions sont requises, utilisez-les, sinon laissez le tableau d'actions vide.

${resultGlobal && resultGlobal.length > 0 ? "context: {context};" : "{context}"}
${ChatHistory && ChatHistory.length > 0 ? "history: {ChatHistory};" : "{ChatHistory}"}
${resultConversations && resultConversations.length > 0 ? "memory: {conversation};" : "{conversation}"}
camera: {camera};
actions: {syntaxAction};

répond à la question: {question}`);
        
console.log(`Répondez à la question suivante en format JSON. Soyez clair et concis. Si des actions sont requises, utilisez-les, sinon laissez le tableau d'actions vide.
${resultGlobal && resultGlobal.length > 0 ? "context: "+resultGlobal+";" : ""}
${ChatHistory && ChatHistory.length > 1 ? "history: "+ChatHistory+";" : ""}
${resultConversations && resultConversations.length > 0 ? "memory: "+resultConversations+";" : "{conversation}"}
actions: ${resultActions};
camera: ${response_camera};

répond à la question: ${prompt}:`);
        
        const PROMPT_TEMPLATE = PromptTemplate.fromTemplate(textTemplate);
        

        const chain = RunnableSequence.from([
            async (input) => ({
                    context: resultGlobal,
                    conversation: resultConversations,
                    ChatHistory: JSON.stringify(ChatHistory),
                    syntaxAction: resultActions,
                    camera: response_camera,
                    question: input.question
                }),
                PROMPT_TEMPLATE,
                model,

                new StringOutputParser()
        ]);

        if (configConst.stream) {
            const finalResult = await chain.stream({
                context: resultGlobal,
                conversation: resultConversations,
                syntaxAction: resultActions,
                ChatHistory: ChatHistory,
                camera: response_camera,
                question: prompt
            });

            process.stdout.write('\n\x1b[1mRéponse:\x1b[0m\x1b[36m ');
            let chunks = []
            for await (const chunk of finalResult) process.stdout.write(chunk), chunks.push(chunk);
            process.stdout.write('\x1b[0m\n');

            ChatHistory.push(`AI: ${chunks.join('').replace(/\n|\r/g, '')}`)

            console.log(`Temps de génération: ${((Date.now()-Timer)/1000)}s`)

            if (configConst.serial === true && JSON.parse(chunks.join('')) && JSON.parse(chunks.join('')).actions) {
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

            if (configConst.tts) {
                try {
                    // console.log(res)
                    tts(res?.message) //Text to speech
                } catch (err) {
                    console.log('impossible de formatter le contenu de la réponse')
                }
            }

            return {result: res}
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