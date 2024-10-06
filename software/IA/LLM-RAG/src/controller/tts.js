import axios from 'axios';

export default async (text) => {
  if (!text) {
    return console.error('Impossible de charger le texte !');
  }

  try {
    const response = await axios.post('http://127.0.0.1:5000/synthesize', {
      phrase: text
    });
    console.log(response.data.message);
  } catch (error) {
    console.error('Erreur lors de la synthèse :', error);
  }
};