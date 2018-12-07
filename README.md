# FlowmMeter
An ESP8266 based project to monitor the flow of water (or any fluid, for that matter) in a pipe using a flow sensor. Also Android app to display the info<br>
The firmware is a mess, but it works.<br>
The app is <b>incredibly</b> low effort too. The IP address for the ESP8266 is hard coded.<br>
<img src="https://raw.githubusercontent.com/ShrinathN/FlowMeter/master/img/app.png"><br><br>
The above is the app <br><br>
<img src="https://raw.githubusercontent.com/ShrinathN/FlowMeter/master/img/circuit.png"><br><br>
I used an ESP8266-01 for this, but you're welcome to use some other model, of you wish. You will have to edit <i>interrupt_functions.h</i> file to accomplish this.
