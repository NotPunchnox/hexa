import { PromptTemplate } from "@langchain/core/prompts";
import { ChatOllama } from "@langchain/ollama";
import { RunnableSequence } from "@langchain/core/runnables";
import { z } from "zod";

const model = new ChatOllama({
    model: "Aash:3b",
});

const schemaForTool = z.object({
    message: z.string().describe("output message"),
    actions: z.array(z.string()).describe("output actions for robot"),
});

const textTemplate = `
You are assisting a robot. Use the following context:
Context: {context}

Now answer the following question, Ensure you use the 'structuredResponse' tool:
Question: {question}
`;

const PROMPT_TEMPLATE = PromptTemplate.fromTemplate(textTemplate);

const llmWithStructuredOutput = model.withStructuredOutput(schemaForTool, {
    name: "structuredResponse",
});

const chain = RunnableSequence.from([
    async (input) => {
        return {
            context: "Les hexapodes sont des robots à six pattes.",
            question: input,
        };
    },
    PROMPT_TEMPLATE,
    llmWithStructuredOutput,
]);

const input = "Quels mouvements le robot peut-il faire ?";

const res = await chain.stream(input);

for await (const chunk of res) {
    process.stdout.write(String(chunk));
}
