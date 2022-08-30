var xmlHttp=createXmlHttpObject();
function createXmlHttpObject(){
 if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
 }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
 }
 return xmlHttp;
}

function getModeNameByID(mID)
   {
     switch (mID)
	 {
	   case 0: return "Режим эффектов: цвет"; break;
	   case 1: return "Режим эффектов: все"; break;
	   case 2: return "Режим эффектов: радуга"; break;
	   case 3: return "Режим эффектов: театр"; break;
	   case 4: return "Режим эффектов: пламя"; break;
	   case 5: return "Режим эффектов: ночник"; break;
	   case 6: return "Режим эффектов: выключено"; break;
	   default: return mID; break;
	 }
   }

function GetSecondNameByID(sID)
{
     switch (sID)
	 {
	   case 0: return "Вторая зона: выключено"; break;
	   case 1: return "Вторая зона: включено"; break;
	   default: return sID; break;
	 }	
}


function load(){
 if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
  xmlHttp.open('PUT','/config.json',true);
  xmlHttp.send(null);
  xmlHttp.onload = function(e) {
   jsonResponse=JSON.parse(xmlHttp.responseText);
   //loadBlock();
   if (document.getElementById('cM') != null)
   {
     document.getElementById('cM').innerHTML = getModeNameByID(jsonResponse["cM"]);
   }
   if (document.getElementById('cM1') != null)
   {   
	 document.getElementById('cM1').innerHTML = GetSecondNameByID(jsonResponse["cM1"]);
   }
   if (document.getElementById('rVal') != null)
   {   
     document.getElementById('rVal').value = jsonResponse["rVal"];
   }
   if (document.getElementById('gVal') != null)
   {   
     document.getElementById('gVal').value = jsonResponse["gVal"];
   }
   if (document.getElementById('bVal') != null)
   {   
     document.getElementById('bVal').value = jsonResponse["bVal"];
   }
   if (document.getElementById('r1Val') != null)
   {   
     document.getElementById('r1Val').value = jsonResponse["r1Val"];
   }
   if (document.getElementById('g1Val') != null)
   {   
     document.getElementById('g1Val').value = jsonResponse["g1Val"];
   }
   if (document.getElementById('b1Val') != null)
   {   
     document.getElementById('b1Val').value = jsonResponse["b1Val"];
   }
   if (document.getElementById('r2Val') != null)
   {   
     document.getElementById('r2Val').value = jsonResponse["r2Val"];
   }
   if (document.getElementById('g2Val') != null)
   {   
     document.getElementById('g2Val').value = jsonResponse["g2Val"];
   }
   if (document.getElementById('b2Val') != null)
   {   
     document.getElementById('b2Val').value = jsonResponse["b2Val"];
   }
   if (document.getElementById('ssdp_t') != null)
   {   
     document.getElementById('ssdp_t').innerHTML = jsonResponse["SSDP"];
   }
      if (document.getElementById('ssdp') != null)
   {   
   document.getElementById('ssdp').value = jsonResponse["SSDP"];
   }
   if (document.getElementById('ssidap') != null)
   {   
     document.getElementById('ssidap').value = jsonResponse["ssidAP"];
   }
   if (document.getElementById('passwordap') != null)
   {   
     document.getElementById('passwordap').value = jsonResponse["passwordAP"];
   }
   if (document.getElementById('ssid') != null)
   {   
     document.getElementById('ssid').value = jsonResponse["ssid"];
   }
   if (document.getElementById('password') != null)
   {   
   document.getElementById('password').value = jsonResponse["password"];
   }
   if (document.getElementById('timezone') != null)
   {   
   document.getElementById('timezone').value = jsonResponse["timezone"];
   }
   if (document.getElementById('idx') != null)
   {   
   document.getElementById('idx').value = jsonResponse["idx"];
   }
   if (document.getElementById('date') != null)
   {   
     document.getElementById('date').innerHTML = jsonResponse["date"]; 
   }
   if (document.getElementById('time') != null)
   {   
     document.getElementById('time').innerHTML = jsonResponse["time"]; 
   }
   if (document.getElementById('pnum') != null)
   {   
     document.getElementById('pnum').value = jsonResponse["pnum"];
   }
   if (document.getElementById('peffects') != null)
   {   
     document.getElementById('peffects').value = jsonResponse["peffects"];
   }
   
  //io
  
   if (document.getElementById('STRIPPIN') != null)
   {   
     document.getElementById('STRIPPIN').value = jsonResponse["STRIPPIN"];
   }
   if (document.getElementById('IOLEDPIN') != null)
   {   
     document.getElementById('IOLEDPIN').value = jsonResponse["IOLEDPIN"];
   }
   if (document.getElementById('BOOTLEDPIN') != null)
   {   
     document.getElementById('BOOTLEDPIN').value = jsonResponse["BOOTLEDPIN"];
   }
   if (document.getElementById('RELAYPIN4') != null)
   {   
     document.getElementById('RELAYPIN4').value = jsonResponse["RELAYPIN4"];
   }
   if (document.getElementById('BUTTONPIN') != null)
   {   
     document.getElementById('BUTTONPIN').value = jsonResponse["BUTTONPIN"];
   }
   if (document.getElementById('PWMPIN') != null)
   {   
     document.getElementById('PWMPIN').value = jsonResponse["PWMPIN"];
   }
   if (document.getElementById('PIN_SDA') != null)
   {   
     document.getElementById('PIN_SDA').value = jsonResponse["PIN_SDA"];
   }
   if (document.getElementById('PIN_SCL') != null)
   {   
     document.getElementById('PIN_SCL').value = jsonResponse["PIN_SCL"];
   }
   if (document.getElementById('IO_inversed') != null)
   {   
     document.getElementById('IO_inversed').value = jsonResponse["IO_inversed"];
	 if (jsonResponse["IO_inversed"]==1)
	 {
	   document.getElementById("IO_inversed").checked = true;
	 }
	 document.getElementById('IO_inversed').value = "Инверсно";
   }
   if (document.getElementById('BOOT_inversed') != null)
   {   
	 if (jsonResponse["BOOT_inversed"]==1)
	 {
	   document.getElementById("BOOT_inversed").checked = true;
	   
	 }
	 document.getElementById('BOOT_inversed').value = "Инверсно";
   }
   if (document.getElementById('usaled') != null)
   {   
	 if (jsonResponse["usaled"]==1)
	 {
	   document.getElementById("usaled").checked = true;
	   document.getElementById('usaled').value = 1;
	 }
	 else
	 {
		document.getElementById('usaled').value = 0;
	 }
	 
	 //document.getElementById('usaled').value = "Инверсно";
   }

   if (document.getElementById('c1_border') != null)	   
   {
	 _r1 = jsonResponse["rVal"];
     _g1 = jsonResponse["gVal"];
     _b1 = jsonResponse["bVal"];
     //console.log(_r1);
     c1.setRgb({ r: _r1, g: _g1, b: _b1 }); 
     document.getElementById('c1_border').style.borderBottom = "20px solid " + c1.hex;
   }
   if (document.getElementById('c2_border') != null)
   {
     _r2 = jsonResponse["r1Val"];
     _g2 = jsonResponse["g1Val"];
     _b2 = jsonResponse["b1Val"];
     //console.log(_r2);
     c2.setRgb({ r: _r2, g: _g2, b: _b2 });         
     document.getElementById('c2_border').style.borderBottom = "20px solid " + c2.hex;
   } 
   if (document.getElementById('c3_border') != null)
   {
     _r3 = jsonResponse["r2Val"];
     _g3 = jsonResponse["g2Val"];
     _b3 = jsonResponse["b2Val"];     
     c3.setRgb({ r: _r3, g: _g3, b: _b3 });         
     document.getElementById('c3_border').style.borderBottom = "20px solid " + c3.hex;
   } 

  }
 }
}


function loadBlock(data2) {
 data2 = JSON.parse(xmlHttp.responseText);
 data = document.getElementsByTagName('body')[0].innerHTML;
 var new_string;
for (var key in data2) {
	var ModeName;	
	var SecondName;
	if (key=="cM") {		
       ModeName = getModeNameByID(data2[key]);
       new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), ModeName);
	}
    if (key=="cM1") {		
        ModeName = GetSecondNameByID(data2[key]);	   
        new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), ModeName);
	}
	if ((key!="cM") && (key!="cM1"))
	{
      new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);		
	}
 data = new_string;
}
 document.getElementsByTagName('body')[0].innerHTML = new_string;
 handleServerResponse();
}

function val(id){
 var v = document.getElementById(id).value;
 return v;
}
function send_request(submit,server){
 request = new XMLHttpRequest();
 request.open("GET", server, true);
 request.send();
 save_status(submit,request);
}
function save_status(submit,request){
 old_submit = submit.value;
 request.onreadystatechange = function() {
  if (request.readyState != 4) return;
  submit.value = request.responseText;
  setTimeout(function(){
   submit.value=old_submit;
   submit_disabled(false);
  }, 1000);
 }
 submit.value = 'Подождите...';
 submit_disabled(true);
}
function submit_disabled(request){
 var inputs = document.getElementsByTagName("input");
 for (var i = 0; i < inputs.length; i++) {
  if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
 }
}
function toggle(target) {
 var curVal = document.getElementById(target).className;
 document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}
