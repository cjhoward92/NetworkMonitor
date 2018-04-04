// This script acts as a client to call to a server instance
const net = require('net');

const runClient = () => new Promise((resolve, reject) => {
  let resolved = false;
  const client = net.createConnection({ port: '36000' }, () => {
    console.log('client connected!');
    client.write('hello darkness my old friend!\n');
  });
  client.on('data', (data) => {
    console.log('data:', data.toString());
    client.end();
  });
  client.on('end', () => {
    if (resolved) return;
    
    console.log('disconnected!');
    resolved = true;
    resolve();
  });
  client.on('error', (err) => {
    if (resolved) return;

    console.log('error:', err);
    resolved = true;
    reject(err);
  })
});

runClient().then(() => console.log('done!'), () => console.log('done!'));