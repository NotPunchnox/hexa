import ollama
import json
import time
import os
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity

# Path to the conversation data file
conversation_file = './models/model-format.json'
embeddings_file = './models/embeddings.json'

# Load the existing conversation data if available
try:
    with open(conversation_file, 'r', encoding='UTF-8') as file:
        conversation = json.load(file)
except FileNotFoundError:
    conversation = []

# Load the existing embeddings data if available
try:
    with open(embeddings_file, 'r', encoding='UTF-8') as file:
        embeddings = json.load(file)
except FileNotFoundError:
    embeddings = []

# Function to get embeddings from Ollama
def get_embedding(prompt):
    response = ollama.embeddings(model="mxbai-embed-large", prompt=prompt)
    return response["embedding"]

def find_most_relevant_doc(embedding, embeddings, documents):
    if not embeddings:
        return "Aucune conversation précédente trouvée."

    similarities = cosine_similarity([embedding], embeddings)[0]
    most_similar_index = np.argmax(similarities)
    return documents[most_similar_index]

while True:
    prompt = input('\x1b[33mQuestion: \x1b[33m')
    if prompt == "exit":
        print('Goodbye!')
        os._exit(0)

    # Generate an embedding for the prompt
    prompt_embedding = get_embedding(prompt)

    # Retrieve the most relevant document
    data = find_most_relevant_doc(prompt_embedding, embeddings, conversation)
    print(f"\x1b[35mAncienne conversation: {data}\x1b[0m")

    # Start the timer
    start_time = time.time()
    promptFinal = f"""
    Tu es un robot et tu parles avec une personne.
    La question posée te rappelle cette conversation: {data}.

    La question est la suivante: {prompt}

    Complète correctement la syntaxe des actions si tu dois en faire; Utilise seulement les actions que tu as apprises.
    Ton output: """

    # Generate the response
    output = ollama.generate(
        model="Test",
        prompt=promptFinal
    )

    # Stop the timer
    end_time = time.time()
    response_time = end_time - start_time

    result_message = output['response']
    print(result_message)
    print(f"\x1b[32mTemps de génération de la réponse: {response_time:.2f} secondes\x1b[0m")

    # Save the prompt and result in the conversation list
    combined_message = """{
        "input": """+prompt+""",
        "output": """+result_message+"""
    }"""
    conversation.append(combined_message)

    # Save the updated conversation to JSON file
    with open(conversation_file, 'w', encoding='UTF-8') as file:
        json.dump(conversation, file, indent=4, ensure_ascii=False)

    # Generate and save the embedding
    new_embedding = get_embedding(json.dumps(combined_message))
    embeddings.append(new_embedding)

    # Save the updated embeddings to JSON file
    with open(embeddings_file, 'w', encoding='UTF-8') as file:
        json.dump(embeddings, file, indent=4, ensure_ascii=False)
