import React, { useEffect, useState } from 'react';
import { db } from '../Services/firebase';
import { onValue, ref } from "firebase/database";
import Container from '@mui/material/Container';
import Grid from '@mui/material/Grid';
import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';

const System = () => {
  const [clientData, setClientData] = useState({});
  const [serverData, setServerData] = useState({});

  useEffect(() => {
    const clientRef = ref(db, 'client');
    const serverRef = ref(db, 'server');

    const unsubscribeClient = onValue(clientRef, (snapshot) => {
      const data = snapshot.val();
      if (snapshot.exists()) {
        setClientData(data);
      }
    });

    const unsubscribeServer = onValue(serverRef, (snapshot) => {
      const data = snapshot.val();
      if (snapshot.exists()) {
        setServerData(data);
      }
    });

    return () => {
      unsubscribeClient();
      unsubscribeServer();
    };
  }, []);

  const getTemperatureColor = (value) => {
    const normalizedValue = (value - 10) / (40 - 10);
    const red = Math.round(255 * normalizedValue);
    const green = Math.round(255 * (1 - normalizedValue));
    return `rgb(${red}, ${green}, 0)`;
  };

  const getHumidityColor = (value) => {
    const normalizedValue = (value +23) / 50;
    const blue = Math.round(255 * normalizedValue);
    return `rgb(0, 0, ${blue})`;
  };

  const getGasColor = (value) => {
    const normalizedValue = (value - 200) / (800 - 200);
    const red = Math.round(255 * normalizedValue);
    const green = Math.round(255 * (1 - normalizedValue));
    return `rgb(${red}, ${green}, 0)`;
  };

  return (
    <Container>
      <Grid item xs={6}>
          <Card>
            <CardContent>
              <h2>Client Data</h2>
              <Container style={{display: 'flex', justifyContent: 'center', alignItems:'center'}}>
              <Card m={2} style={{ margin: '5px', width:'300px', backgroundColor: getTemperatureColor(serverData.temp), textAlign: 'center' }}>
                <CardContent>
                  <p>{clientData.temp} °C</p>
                </CardContent>
              </Card>
              </Container>
              <Container style={{display: 'flex', justifyContent: 'center', alignItems:'center'}}>

              <Card m={2} style={{ color:'white' , margin: '5px', width:'300px',backgroundColor: getHumidityColor(serverData.hum), textAlign: 'center' }}>
                <CardContent>
                  <p>{clientData.hum} %</p>
                </CardContent>
              </Card>
              </Container>

              <Container style={{display: 'flex', justifyContent: 'center', alignItems:'center'}}>

              <Card m={2} style={{ margin: '5px',width:'300px', backgroundColor: getGasColor(serverData.gas), textAlign: 'center' }}>
                <CardContent>
                  <p>{clientData.gas} ppm</p>
                </CardContent>
              </Card>
              </Container>

            </CardContent>
          </Card>
        </Grid>
        <Grid item xs={6}>
          <Card>
            <CardContent>
              <h2>Server Data</h2>
              <Container style={{display: 'flex', justifyContent: 'center', alignItems:'center'}}>
              <Card m={2} style={{ margin: '5px', width:'300px', backgroundColor: getTemperatureColor(serverData.temp), textAlign: 'center' }}>
                <CardContent>
                  <p>{serverData.temp} °C</p>
                </CardContent>
              </Card>
              </Container>
              <Container style={{display: 'flex', justifyContent: 'center', alignItems:'center'}}>

              <Card m={2} style={{ color:'white' , margin: '5px', width:'300px',backgroundColor: getHumidityColor(serverData.hum), textAlign: 'center' }}>
                <CardContent>
                  <p>{serverData.hum} %</p>
                </CardContent>
              </Card>
              </Container>

              <Container style={{display: 'flex', justifyContent: 'center', alignItems:'center'}}>

              <Card m={2} style={{ margin: '5px',width:'300px', backgroundColor: getGasColor(serverData.gas), textAlign: 'center' }}>
                <CardContent>
                  <p>{serverData.gas} ppm</p>
                </CardContent>
              </Card>
              </Container>

            </CardContent>
          </Card>
        </Grid>
    </Container>
  );
};

export default System;