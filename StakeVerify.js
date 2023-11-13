//----------Stake-Verification-------------

import crypto from "crypto";
import _ from "lodash";
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
      //console.log(`\n----------\nindex:${i}\nvalue:${value}\ndivider:${divider}\npartialResult:${partialResult}\nresult:${result}\n----------`);
      return result + partialResult;
    }, 0)
  );
};

function result(obj){
    let res = generateFloats(obj);
    res = ((Math.trunc((res[0]*1000100)/100))/100).toFixed(2);
    console.log(res);
}


let obj = {
		serverSeed :"532082a5cad971712b3b076bddc84496a13da9c3eea977cf6bfd7f043cebcf06"	,

	clientSeed :"K6-DB0Htbs"    ,

	nonce      :  5523 ,

	cursor     :   0 ,

	count      :  1
	
}


result(obj);


