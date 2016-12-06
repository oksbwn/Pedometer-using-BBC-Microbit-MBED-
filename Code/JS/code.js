// When the BBC micro:bit runs.
function onStart(  ) {
	globals.temp = 0;
	globals.result = 0;
	globals.lastAvgReading = 0;
	globals.avgReading = 0;
	globals.flag = 0;
	globals.steps = 0;
	while (true) {
		
		microbit.calculateAccln();
		microbit.squareRoot();
		globals.avgReading = ( globals.lastAvgReading + globals.result ) / 2;
		globals.lastAvgReading = globals.avgReading;
		if (( globals.avgReading > 900 ) && ( globals.flag == 0 )) {
			globals.steps = globals.steps + 1;
		}
		
		else if (( globals.avgReading > 900 ) && ( globals.flag == 1 )) {	
			
		}
		
		if (( globals.avgReading < 900 ) && ( globals.flag == 1 )) {
			
			globals.flag == 0
			
		}
		
		microbit.say(globals.steps);
		microbit.writeNumberSerial(globals.steps);
		//	microbit
		
	}
	
	
}

function squareRoot(  ) {
	var number = globals.temp;
	//	microbit.say(number);
	var x = 0;
	var count = 0;
	for (var counter = 2; x <= number; counter = counter + 2) {
		
		x = x + counter;
		count = count + 1;
		
	}
	
	globals.result = count;
	
}

function calculateAccln(  ) {
	globals.temp = ( Math.pow(microbit.accelerationX, 2) + Math.pow(microbit.accelerationY, 2) ) + Math.pow(microbit.accelerationZ, 2);
	
}
