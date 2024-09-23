import {
    OllamaEmbeddings
} from "@langchain/community/embeddings/ollama";
import {
    MemoryVectorStore
} from "langchain/vectorstores/memory";
import config from "../../config.json" assert {type: "json"}
// import config from "../../examples/client.js"
import textSplitter, {
    Split
} from "./textSplitter.js";

const embeddings = new OllamaEmbeddings({
    model: config.EMBEDDING_MODEL,
    baseUrl: config.OLLAMA_API_URL
});

const AddDoc = async (texts, vectorStore) => {

    try {
        if (!vectorStore) {
            vectorStore = await MemoryVectorStore.fromDocuments(texts, embeddings);
            console.log('\x1b[32mNew VectorStore initialized with texts: ', texts, '\x1b[0m');
        } else {
            await vectorStore.addDocuments(await Split(texts));
            console.log('\x1b[32mNew texts added to VectorStore: ', texts, '\x1b[0m');
        }
    } catch (error) {
        console.error('\x1b[31mError adding documents to vector store:', error, '\x1b[0m');
        throw error;
    }
}

export {
    AddDoc
};

export default async () => {

    try {
        const {
            documentsActions,
            documentsGlobal,
            documentsConversations
        } = await textSplitter()

        const vectorStoreActions = new MemoryVectorStore(embeddings)
        await vectorStoreActions.addDocuments(documentsActions)
        // const vectorStoreActions = await MemoryVectorStore.fromDocuments(documentsActions, embeddings);
        console.log('\x1b[32mVector store for Actions initialized successfully.\x1b[0m');

        const vectorStoreGlobal = new MemoryVectorStore(embeddings)
        await vectorStoreGlobal.addDocuments(documentsGlobal)
        // const vectorStoreGlobal = await MemoryVectorStore.fromDocuments(documentsGlobal, embeddings);
        console.log('\x1b[32mVector store for Global-Memory initialized successfully.\x1b[0m');

        const vectorStoreConversations = new MemoryVectorStore(embeddings)
        await vectorStoreConversations.addDocuments(documentsConversations)
        // const vectorStoreConversations = await MemoryVectorStore.fromDocuments(documentsConversations, embeddings);
        console.log('\x1b[32mVector store for Conversations initialized successfully.\x1b[0m');


        return {
            vectorStoreActions,
            vectorStoreConversations,
            vectorStoreGlobal
        };

    } catch (error) {
        console.error('\x1b[31mError initializing vector store:', error, '\x1b[0m');
        throw error;
    }
};