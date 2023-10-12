
import crypto from "crypto";
import _ from "lodash";

const verify = (server_seed, client_seed, nonce) => { 
    const hash = crypto.createHmac('sha256',`${client_seed}_${nonce}`).update(server_seed).digest("hex");
    console.log(hash);	
    let index = 0;	
    let lucky = parseInt(hash.substr(index,5), 16);console.log(lucky);
    while (lucky >= 1000000) { 
        lucky = parseInt(hash.substr(index, 5), 16); index += 5; 
	 }
	console.log(lucky);   
	console.log((lucky%10000/100).toFixed(2));
  return (lucky % 10000 / 100).toFixed(2); 
} 


let server = "511c5597e62322ed0bb161c1858bdf5a82bcfea595d7767912f87f3108819cd1",

    client = "544a6fecf7c91692527079045df5663f",

    nonce  =  317334 ;


//verify(server,client,nonce);

/*
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

// Random number generation based on following inputs: serverSeed, clientSeed, nonce and cursor
function byteGenerator1({ serverSeed, clientSeed, nonce, cursor }) {
  // Setup curser variables
  let currentRound = Math.floor(cursor / 32);
  let currentRoundCursor = cursor;
  currentRoundCursor -= currentRound * 32;

  // Generate outputs until cursor requirement fullfilled
  while (true) {
    // HMAC function used to output provided inputs into bytes
    const hmac = createHmac('sha256', serverSeed);
    hmac.update(`${clientSeed}:${nonce}:${currentRound}`);
    const buffer = hmac.digest();

    // Update curser for next iteration of loop
    while (currentRoundCursor < 32) {
      //yield Number(buffer[currentRoundCursor]);
      currentRoundCursor += 1;
    }
    currentRoundCursor = 0;
    currentRound += 1;
  }
}


// Convert the hash output from the rng byteGenerator to floats
function generateFloats1 ({ serverSeed, clientSeed, nonce, cursor, count }) {
  // Random number generator function
  const rng = byteGenerator1({ serverSeed, clientSeed, nonce, cursor });
  // Declare bytes as empty array
  const bytes = [];

  // Populate bytes array with sets of 4 from RNG output
  while (bytes.length < count * 4) {
    bytes.push(rng.next().value);
  }

  // Return bytes as floats using lodash reduce function
  return _.chunk(bytes, 4).map(bytesChunk =>
    bytesChunk.reduce((result, value, i) => {
      const divider = 256 ** (i + 1);
      const partialResult = value / divider;
      return result + partialResult;
    }, 0)
  );
};

/*
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/


// Random number generation based on following inputs: serverSeed, clientSeed, nonce and cursor
function* byteGenerator({ serverSeed, clientSeed, nonce, cursor }) {
  // Setup curser variables
  let currentRound = Math.floor(cursor / 32);
  let currentRoundCursor = cursor;
  currentRoundCursor -= currentRound * 32;

  // Generate outputs until cursor requirement fullfilled
  while (true) {
   
	const buffer = crypto.createHmac('sha256',serverSeed).update(`${clientSeed}:${nonce}:${currentRound}`).digest();

    // Update curser for next iteration of loop
    while (currentRoundCursor < 32) {
      yield Number(buffer[currentRoundCursor]);
      currentRoundCursor += 1;
    }
    currentRoundCursor = 0;
    currentRound += 1;
  }
}


// Convert the hash output from the rng byteGenerator to floats
function generateFloats({ serverSeed, clientSeed, nonce, cursor, count }) {
  // Random number generator function
  const rng = byteGenerator({ serverSeed, clientSeed, nonce, cursor });
  // Declare bytes as empty array
  const bytes = [];

  // Populate bytes array with sets of 4 from RNG output
  while (bytes.length < count * 4) {
    bytes.push(rng.next().value);
  }

  // Return bytes as floats using lodash reduce function
  return _.chunk(bytes, 4).map(bytesChunk =>
    bytesChunk.reduce((result, value, i) => {
      const divider = 256 ** (i + 1);
      const partialResult = value / divider;
      return result + partialResult;
    }, 0)
  );
};


let obj = {
		serverSeed :"38a2aa003bbf39f0bb15e9fe700806cf566f6b9bf631af26d4a2745df8c3274e"	,

	clientSeed :"0CUtKAUzVb"    ,

	nonce      :  45973 ,

	cursor     :   0 ,

	count      :  1
	
}


let res = generateFloats(obj);

console.log(res);

console.log(((res[0]*10001)/100).toPrecision(4));

console.log(Math.round((((res[0]*10001)/100))* 100)/100);

