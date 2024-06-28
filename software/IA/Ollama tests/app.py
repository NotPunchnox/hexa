import ollama
import json
import chromadb
import time
import os

# Path to the conversation data file
conversation_file = './models/model-format.json'

# Load the existing conversation data if available
try:
    with open(conversation_file, 'r', encoding='UTF-8') as file:
        conversation = json.load(file)
except FileNotFoundError:
    conversation = []

# Initialize ChromaDB client and collection
client = chromadb.Client()
collection = client.create_collection(name="conversation")

# Store each document in a vector embedding database
for i, message in enumerate(conversation):
    response = ollama.embeddings(model="mxbai-embed-large", prompt=message)
    embedding = response["embedding"]
    collection.add(
        ids=[str(i)],
        embeddings=[embedding],
        documents=[json.dumps(message)]
    )

while True:
    prompt = input('\x1b[33mQuestion: \x1b[33m')
    if prompt == "exit":
        print('Goodbye!')
        os._exit(0)

    # Generate an embedding for the prompt and retrieve the most relevant doc
    response = ollama.embeddings(
        prompt=prompt,
        model="mxbai-embed-large"
    )
    results = collection.query(
        query_embeddings=[response["embedding"]],
        n_results=1
    )
    data = results['documents'][0][0]
    print(f"\x1b[35mAncienne conversation: {data}\x1b[0m")

    # Start the timer
    start_time = time.time()
    promptFinal = f"""
    Tu es un robot et tu parles avec une personne.
    La question posée te rappelle cette conversation: {data}.

    La question est la suivante: {prompt}

    Complète correctement la syntaxe des actions si tu dois en faire; Utilise seulements les actions que as appris.
    Ton output: """

    # Generate the response
    output = ollama.chat(
        model="TestQwen",
        messages=[{"role": "user", "content": promptFinal}]
    )

    # Stop the timer
    end_time = time.time()
    response_time = end_time - start_time

    result_message = output['message']['content']
    print(result_message)
    print(f"\x1b[32mTemps de génération de la réponse: {response_time:.2f} secondes\x1b[0m")

    # Save the prompt and result in the conversation list
    combined_message = f"""{"{"}"input": "{prompt}","output": "{result_message}"{"}"}"""
    conversation.append(combined_message)

    # Save the updated conversation to JSON file
    with open(conversation_file, 'w', encoding='UTF-8') as file:
        json.dump(conversation, file, indent=4, ensure_ascii=False)

    # Now use ChromaDB to store the embedding
    json_string = json.dumps(combined_message)
    response = ollama.embeddings(model="mxbai-embed-large", prompt=json_string)
    embedding = response["embedding"]
    collection.add(
        ids=[str(len(conversation) - 1)],
        embeddings=[embedding],
        documents=[json_string]
    )
