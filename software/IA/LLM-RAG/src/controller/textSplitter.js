import { RecursiveCharacterTextSplitter } from "@langchain/textsplitters";
import { JSONLoader } from "langchain/document_loaders/fs/json";
import fs from "fs";
import path from "path";

const DirActions = path.resolve("./src/Training Data/syntax");
const DirGlobal = path.resolve("./src/Training Data/global");
const DirConversations = path.resolve("./src/Training Data/conversations");

const processFiles = async (dir) => {
  const files = fs.readdirSync(dir);
  let documents = [];

  for (const file of files) {
    const filePath = path.join(dir, file);
    const ext = path.extname(file).toLowerCase();

    if (ext === '.txt' || ext === '.md') {
      const text = fs.readFileSync(filePath, 'utf8');
      const splitter = new RecursiveCharacterTextSplitter({
        chunkSize: 100,
        chunkOverlap: 0,
        separators: ["|", "##", ">", "-"],
      });
      const doc = await splitter.createDocuments([text]);
      documents = documents.concat(doc);
    } else if (ext === '.json') {
      let loader;

      if(dir.includes('conversation')) {
        loader = new JSONLoader(filePath); // Ajuster les chemins selon vos besoins
      } else loader = new JSONLoader(filePath)
      const docs = await loader.load();
      documents = documents.concat(docs);
    } else {
      console.warn(`Unsupported file type: ${file}`);
    }
  }
  return documents;
};

export default async () => {
  try {
    // Process all directories
    const documentsActions = await processFiles(DirActions);
    const documentsGlobal = await processFiles(DirGlobal);
    const documentsConversations = await processFiles(DirConversations);

    console.log('\x1b[32mDocuments created\x1b[0m');
    console.log(`Total documents (Actions): ${documentsActions.length}`);
    console.log(`Total documents (Global): ${documentsGlobal.length}`);
    console.log(`Total documents (Conversations): ${documentsConversations.length}`);

    
    return {
      documentsActions,
      documentsGlobal,
      documentsConversations
    };
  } catch (error) {
    console.error('\x1b[31mError initializing vector store:', error, '\x1b[0m');
    throw error;
  }
};
