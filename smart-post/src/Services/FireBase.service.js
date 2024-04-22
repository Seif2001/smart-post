import {db} from './firebase';
import { onValue, ref } from "firebase/database";

const fetchDataFromFirebase = async () => {
  try {
    const db = getDatabase(app);
    const clientDataSnapshot = await get(ref(db, 'client'));
    const clientData = clientDataSnapshot.val();

    const serverDataSnapshot = await get(ref(db, 'server'));
    const serverData = serverDataSnapshot.val();

    return { clientData, serverData };
  } catch (error) {
    console.error('Error fetching data from Firebase:', error);
    throw error;
  }
};

export default fetchDataFromFirebase;
