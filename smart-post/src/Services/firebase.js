
import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

  const firebaseConfig = {

    apiKey: "AIzaSyDoinkaVM6q3GkhKC9HAvUmEgJLOZAm7vw",
  
    authDomain: "smart-post-9409b.firebaseapp.com",
  
    databaseURL: "https://smart-post-9409b-default-rtdb.asia-southeast1.firebasedatabase.app",
  
    projectId: "smart-post-9409b",
  
    storageBucket: "smart-post-9409b.appspot.com",
  
    messagingSenderId: "712791092489",
  
    appId: "1:712791092489:web:c489451c9e9ce5fdd137d8"
  
  };
  
const app = initializeApp(firebaseConfig);

export const db = getDatabase(app);