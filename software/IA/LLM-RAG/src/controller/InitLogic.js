import embedding from "./embedding.js"

export default async() => {
    console.clear(), console.log("[43m[4m[1m\x1b[35mOllama RAG nodejs: by punchnox[0m\n"), console.log("[1mLogs:[0m"), await embedding.InitVector();
};