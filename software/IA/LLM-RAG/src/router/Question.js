import {ChatOllama} from "@langchain/community/chat_models/ollama";
import {StringOutputParser} from "@langchain/core/output_parsers";
import {PromptTemplate} from "@langchain/core/prompts";
import {RunnableSequence,RunnablePassthrough} from "@langchain/core/runnables";
import {formatDocumentsAsString} from "langchain/util/document";
import embedding from "../controller/embedding.js";
import config from "../../config.json" assert { type: "json" }
import tts from "../controller/tts.js";

var ChatHistory = []

export default async(prompt, l=3, modelSelected=config.LLM_MODEL) => {

    if(ChatHistory.length > 5) ChatHistory = ChatHistory.slice(2, ChatHistory.length)
    ChatHistory.push(`user: ${prompt}`)

    const model = new ChatOllama({
        baseUrl: config.OLLAMA_API_URL,
        model: modelSelected,
        format: config.format !== "json" ? null : config.format
    });

    try {
        let { resultActions, resultConversations, resultGlobal } = await embedding.search(prompt, l);
        resultActions = formatDocumentsAsString(resultActions);
        resultConversations = formatDocumentsAsString(resultConversations);
        resultGlobal = formatDocumentsAsString(resultGlobal);
        // console.log(resultConversations, resultGlobal, resultActions)

        const textTemplate = `Utilisez les éléments de contexte suivants pour répondre à la question à la fin, avec le format json connue.
Répondez de manière claire et concise en utilisant seulement le format JSON imposé.
Utilisez des actions si nécessaire sinon laissez le tableau vide, n'essayez pas d'en inventer.

----------------
EXTERNAL DATA: {context}
----------------
MEMORY CHAT: {conversation}
----------------
EXAMPLE ACTION: {syntaxAction}
----------------
CHAT HISTORY: {ChatHistory}
----------------
QUESTION: {question}
----------------

Réponse: `;

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

        if(config.stream) {
            const finalResult = await chain.stream({
                context: resultGlobal,
                conversation: resultConversations,
                syntaxAction: resultActions,
                ChatHistory: ChatHistory,
                question: prompt
            });

            process.stdout.write('\n\x1b[1mRéponse:\x1b[0m\x1b[36m');
            let chunks = []
            for await (const chunk of finalResult) process.stdout.write(chunk), chunks.push(chunk);
            process.stdout.write('\x1b[0m\n');

            ChatHistory.push(`AI: ${chunks.join('').replace(/\n|\r/g, '')}`)

            if(config.tts) {
                try {
                    const res = JSON.parse(chunks.join('').replace(/\n|\r/g, ''))
                    console.log(res)
                    tts(res?.message) //Text to speech
                } catch (err) {
                    console.log('impossible de formatter le contenu de la réponse')
                }
            }
            

        } else {
            const finalResult = await chain.invoke({
                context: resultGlobal,
                conversation: resultConversations,
                syntaxAction: resultActions,
                question: prompt
            });
            return finalResult
        }
    } catch (error) {
        console.error('Error:', error);
    }
}