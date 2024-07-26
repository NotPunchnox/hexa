import InitEmbedding from "./initEmbedding.js"

var vectorStore

const InitVector = async() => {
  vectorStore = await InitEmbedding();
}
const getVectorStore = async() => {
  return vectorStore
}

const search = async (prompt, l=3) => {
  try {
    let resultGlobal = []

    if (/rag|hexapod|ia|ai|retrieval|augmented|generation|gpt|llm|paradisier|cinématique|cinematique|inversée|kinematique|kinématique/i.test(prompt)) {
      resultGlobal = await vectorStore.vectorStoreGlobal.similaritySearch(prompt, l);
    }

    const resultActions = await vectorStore.vectorStoreActions.similaritySearch(prompt, 2);
    const resultConversations = await vectorStore.vectorStoreConversations.similaritySearch(prompt, l);

    return {resultActions, resultConversations, resultGlobal}
  } catch (error) {
    console.error('\x1b[31mError during similarity search:', error, '\x1b[0m');
  }
};

export default {
    InitEmbedding,
    search,
    InitVector,
    getVectorStore
};