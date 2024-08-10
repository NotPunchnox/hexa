import { OllamaEmbeddings } from "@langchain/community/embeddings/ollama";
import { MemoryVectorStore } from "langchain/vectorstores/memory";
import config from "../../config.json" assert { type: "json" }
import textSplitter from "./textSplitter.js";

const embeddings = new OllamaEmbeddings({
  model: config.EMBEDDING_MODEL,
  baseUrl: config.OLLAMA_API_URL
});

export default async () => {
  try {
    const {
      documentsActions,
      documentsGlobal,
      documentsConversations
    } = await textSplitter()


    const vectorStoreActions = await MemoryVectorStore.fromDocuments(documentsActions, embeddings);
    console.log('\x1b[32mVector store for Actions initialized successfully.\x1b[0m');

    const vectorStoreGlobal = await MemoryVectorStore.fromDocuments(documentsGlobal, embeddings);
    console.log('\x1b[32mVector store for Global-Memory initialized successfully.\x1b[0m');

    const vectorStoreConversations = await MemoryVectorStore.fromDocuments(documentsConversations, embeddings);
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
