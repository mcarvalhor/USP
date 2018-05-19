var FSM={
	"inputs":{},
	"outputs":{},
	"states":{},
	"currentstate":null,
	"resetstate":null,
	"moore":false
};

var MessageBox={
	close: function(cancel, box){
		if(cancel===true)	return;
		$(document.getElementById(box)).stop().slideUp(500);
		$("#MessageBox").stop().fadeOut(1000);
	},
	onok: null,
	onyes: null,
	onno: null
};

var ExitConfirmation=false;
var ExitConfirmationMessage=null;

var IsConditionedState=false;
var ConfigState=null;

var Simulation=false;
var CircleRadius=1;



function AddInput(value){
	var Me=document.createElement("span");
	if(!value)	value="";
	Me.innerHTML='<input type="text" placeholder="Nome da entrada" value="'+value+'" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." required><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a>';
	document.getElementById("Form_Inputs").appendChild(Me);
	Me.firstElementChild.focus();
}

function AddOutput(value){
	var Me=document.createElement("span");
	if(!value)	value="";
	Me.innerHTML='<input type="text" placeholder="Nome da saída" value="'+value+'" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." required><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a>';
	document.getElementById("Form_Outputs").appendChild(Me);
	Me.firstElementChild.focus();
}

function AddState(value){
	var Me=document.createElement("span");
	if(!value)	value="";
	Me.innerHTML='<input type="text" placeholder="Nome (ou codificação) do estado" value="'+value+'" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." required><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a>';
	document.getElementById("Form_States").appendChild(Me);
	Me.firstElementChild.focus();
}
function ResetDescription(){
	document.getElementById("Form_Inputs").innerHTML="";
	document.getElementById("Form_Outputs").innerHTML="";
	document.getElementById("Form_States").innerHTML="";
}

function AddConfig(){
	var Me=document.createElement("span");
	Me.innerHTML='<input type="text" list="FSM_Datalist_Outputs" placeholder="Saída" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." required> será <select><option value="0" selected>0</option><option value="1">1</option></select><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a>';
	document.getElementById("Form_NC_Configs").appendChild(Me);
	Me.firstElementChild.focus();
}

function AddConditionedConfig(){
	var Me=document.createElement("span");
	if(IsConditionedState===true){
		Me.innerHTML='Se <input type="text" placeholder="Condição (Verilog)" pattern="^[a-zA-Z0-9_&|~^\\s]+$" title="Uma equação Verilog válida." required>, o próximo estado será <input type="text" list="FSM_Datalist_States" placeholder="Estado" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." required><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a>';
	}else{
		Me.innerHTML='Se <input type="text" placeholder="Condição (Verilog)" pattern="^[a-zA-Z0-9_&|~^\\s]+$" title="Uma equação Verilog válida." required>, <input type="text" list="FSM_Datalist_Outputs" placeholder="Saída" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." required> será <select><option value="0" selected>0</option><option value="1">1</option></select><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a>';
	}
	document.getElementById("Form_WC_Configs").appendChild(Me);
	Me.firstElementChild.focus();
}

function MoveMeUp(Dom){
	var Me=Dom.parentNode;
	var Parent=Me.parentNode;
	var Previous=Me.previousElementSibling;
	if(Previous!=null)
		Parent.insertBefore(Me, Previous);
}

function MoveMeDown(Dom){
	var Me=Dom.parentNode;
	var Parent=Me.parentNode;
	var Next=Me.nextElementSibling;
	if(Next!=null)
		Parent.insertBefore(Next, Me);
}

function DeleteMe(Dom){
	Dom.parentNode.outerHTML="";
}


function ParseDraggedFile(e){
	e.stopPropagation();
	e.preventDefault();
	e.dataTransfer.dropEffect='copy';
}
function ParseDroppedFile(e){
	e.stopPropagation();
	e.preventDefault();
	ParseFile(e.dataTransfer.files);
}
function ParseFile(evt){
	var FR = new FileReader();
	if(evt[0].size>52428800){ // 50 MiB
		document.getElementById("Form_File").reset();
		document.getElementById("DevelopMachine").style.display="block";
		AlertBox("Arquivo selecionado não é válido (tamanho maior que 50 MiB)!", BreakEvent);
		return;
	}
	if(!(evt[0].type.match(/^\s*$/g) || evt[0].type.match(/(text)/gi))){
		document.getElementById("Form_File").reset();
		document.getElementById("DevelopMachine").style.display="block";
		AlertBox("Arquivo selecionado não é válido (não é arquivo de texto)!", BreakEvent);
		return;
	}
	FR.onload=function(e){
		var i, j, k, Content=e.target.result;
		var IsMoore, NStates, NBits, NInputs, NOutputs;
		var AuxEq, AuxState, AuxNextState;
		var CmbState = { }, CmbInput = { }, CmbOutput = { };
		if(!Content.match(/^\s*[01]\s+[0-9]+\s+[0-9]+\s+[0-9]+\s+(\s|[01Xx])+$/g)){
			document.getElementById("Form_File").reset();
			document.getElementById("DevelopMachine").style.display="block";
			AlertBox("Arquivo selecionado não é válido (não está no formato adequado)!", BreakEvent);
			return;
		}
		if(Content.replace(/^\s*([01])(.|\s)*$/g,"$1")=='1'){
			document.getElementById("fi_machinetype_moore").checked=true;
			IsMoore=true;
		}else{
			document.getElementById("fi_machinetype_mealy").checked=true;
			IsMoore=false;
		}
		Content=Content.replace(/^\s*[01]\s+((.|\s)+)$/g, "$1");
		NStates=parseInt(Content.replace(/^\s*([0-9]+)\s+([0-9]+)\s+([0-9]+)\s+((.|\s)+)$/g,"$1"));
		NInputs=parseInt(Content.replace(/^\s*([0-9]+)\s+([0-9]+)\s+([0-9]+)\s+((.|\s)+)$/g,"$2"));
		NOutputs=parseInt(Content.replace(/^\s*([0-9]+)\s+([0-9]+)\s+([0-9]+)\s+((.|\s)+)$/g,"$3"));
		Content=Content.replace(/^\s*([0-9]+)\s+([0-9]+)\s+([0-9]+)\s+((.|\s)+)$/g,"$4");
		Content=Content.replace(/.*[Xx].*/g, "");
		Content=Content.replace(/\s/g, "");
		NBits=Math.ceil(Math.log(NStates)/Math.log(2));
		for(j=0;j<Content.length;j){
			if(j+2*NBits+NInputs+NOutputs > Content.length)	break;
			AuxState="S_";
			for(i=0;i<NBits;i++){
				AuxState+=(Content.charAt(j++)=='0')?'0':'1';
			}
			if(!CmbState.hasOwnProperty(AuxState)){
				CmbState[AuxState] = { "nextstates": { }, "outputs": { } };
			}
			AuxEq="";
			for(i=0;i<NInputs;i++){
				CmbInput["Entrada_"+(i+1)]="0";
				if(Content.charAt(j++)=="0"){
					AuxEq+="~Entrada_"+(i+1)+"&";
				}else{
					AuxEq+="Entrada_"+(i+1)+"&";
				}
			}
			AuxEq=AuxEq.substr(0, AuxEq.length-1);
			AuxNextState="S_";
			for(i=0;i<NBits;i++){
				AuxNextState+=(Content.charAt(j++)=='0')?'0':'1';
			}
			CmbState[AuxState]["nextstates"][AuxNextState]=AuxEq;
			if(!CmbState.hasOwnProperty(AuxNextState)){
				CmbState[AuxNextState] = { "nextstates": { }, "outputs": { } };
			}
			for(i=0;i<NOutputs;i++){
				CmbOutput["Saida_"+(i+1)]="0";
				if(IsMoore==true){
					CmbState[AuxState]["outputs"]["Saida_"+(i+1)]=(Content.charAt(j++)=='0')?'0':'1';
				}else{
					if(!CmbState[AuxState]["outputs"].hasOwnProperty("Saida_"+(i+1))){
						CmbState[AuxState]["outputs"]["Saida_"+(i+1)] = { };
					}
					CmbState[AuxState]["outputs"]["Saida_"+(i+1)][((Content.charAt(j++)=='0')?0:1).toString()]=AuxEq;
				}
			}
		}
		ResetDescription();
		for(Input in CmbInput){
			AddInput(Input);
		}
		for(Output in CmbOutput){
			AddOutput(Output);
		}
		for(State in CmbState){
			AddState(State);
		}
		GenMachineTable();
		FSM["states"]=CmbState;
		FSM["moore"]=IsMoore;
		Start();
		document.getElementById("Form_File").reset();
	};
	FR.readAsText(evt[0]);
}

function GenMachineTable(){
	var NInputs, NOutputs, NStates, i;
	FSM["inputs"]={ };
	FSM["outputs"]={ };
	FSM["states"]={ };
	document.getElementById("DevelopMachine").style.display="none";
	NInputs=document.getElementById("Form_Inputs").getElementsByTagName("span");
	NOutputs=document.getElementById("Form_Outputs").getElementsByTagName("span");
	NStates=document.getElementById("Form_States").getElementsByTagName("span");
	document.getElementById("Form_Table").innerHTML="";
	document.getElementById("ConfigureBox_WC_Inputs").innerHTML="";
	document.getElementById("FSM_Datalist_Outputs").innerHTML="";
	document.getElementById("FSM_Datalist_States").innerHTML="";
	document.getElementById("Inputs_Bar").innerHTML="";
	document.getElementById("Outputs_Bar").innerHTML="";
	if(NInputs.length<1){
		document.getElementById("DevelopMachine").style.display="block";
		AlertBox("Insira ao menos uma entrada!", BreakEvent);
		return;
	}
	if(NOutputs.length<1){
		document.getElementById("DevelopMachine").style.display="block";
		AlertBox("Insira ao menos uma saída!", BreakEvent);
		return;
	}
	if(NStates.length<1){
		document.getElementById("DevelopMachine").style.display="block";
		AlertBox("Insira ao menos um estado!", BreakEvent);
		return;
	}
	for(i=0;i<NInputs.length;i++){
		if(NInputs[i].firstElementChild.value=="" || FSM["inputs"].hasOwnProperty(NInputs[i].firstElementChild.value)){
			document.getElementById("DevelopMachine").style.display="block";
			AlertBox("Não podem coexistir duas entradas com o mesmo nome!", BreakEvent);
			return;
		}
		FSM["inputs"][NInputs[i].firstElementChild.value]="0";
		document.getElementById("ConfigureBox_WC_Inputs").innerHTML+=NInputs[i].firstElementChild.value + " ";
		document.getElementById("Inputs_Bar").innerHTML+='<tr><td>'+NInputs[i].firstElementChild.value+'</td><td><a href="javascript:void(0);" onclick="document.getElementById(\'InputValue.'+NInputs[i].firstElementChild.value+'\').checked=false;DefineInputs();Clock();">0</a></td><td><span class="f_switch"><input type="checkbox" onchange="DefineInputs();" id="InputValue.'+NInputs[i].firstElementChild.value+'"><label for="InputValue.'+NInputs[i].firstElementChild.value+'"></label></span></td><td><a href="javascript:void(0);" onclick="document.getElementById(\'InputValue.'+NInputs[i].firstElementChild.value+'\').checked=true;DefineInputs();Clock();">1</a></td></tr>';
	}
	for(i=0;i<NOutputs.length;i++){
		if(NOutputs[i].firstElementChild.value=="" || FSM["outputs"].hasOwnProperty(NOutputs[i].firstElementChild.value)){
			document.getElementById("DevelopMachine").style.display="block";
			AlertBox("Não podem coexistir duas saídas com o mesmo nome!", BreakEvent);
			return;
		}
		FSM["outputs"][NOutputs[i].firstElementChild.value]="0";
		document.getElementById("FSM_Datalist_Outputs").innerHTML+=' <option value="'+NOutputs[i].firstElementChild.value+'">';
		document.getElementById("Outputs_Bar").innerHTML+='<tr><td>'+NOutputs[i].firstElementChild.value+'</td><td id="OutputValue.'+NOutputs[i].firstElementChild.value+'">U</td></tr>';
	}
	for(i=0;i<NStates.length;i++){
		if(NStates[i].firstElementChild.value=="" || FSM["states"].hasOwnProperty(NStates[i].firstElementChild.value)){
			document.getElementById("DevelopMachine").style.display="block";
			AlertBox("Não podem coexistir dois estados com o mesmo nome (ou codificação)!", BreakEvent);
			return;
		}
		FSM["states"][NStates[i].firstElementChild.value] = { "nextstates": null, "outputs": null };
		document.getElementById("FSM_Datalist_States").innerHTML+=' <option value="'+NStates[i].firstElementChild.value+'">';
		document.getElementById("Form_Table").innerHTML+='<tr><td><input type="radio" name="reset_state" value="'+NStates[i].firstElementChild.value+'" title="Este é o estado inicial e definido após reset?" '+((i==0)?'checked':'')+'></td><td>'+NStates[i].firstElementChild.value+'</td><td><a href="javascript:void(0);" onclick="SetNextStatesFor(\''+NStates[i].firstElementChild.value+'\');">Definir...</a></td><td><a href="javascript:void(0);" onclick="SetOutputsFor(\''+NStates[i].firstElementChild.value+'\');">Definir...</a></td></tr>';
	}
	if(document.getElementById("fi_machinetype_moore").checked==true){
		FSM["moore"]=true;
	}else{
		FSM["moore"]=false;
	}
	MouseScroll("#Form_Table tr td:nth-child(2)");
	$("#DevelopMachineArchitecture").stop().fadeIn(500);
}

function Start(){
	var State, len=0, R=100+50, Xc=0, Yc=0, i, Angle;
	document.getElementById("DevelopMachineArchitecture").style.display="none";
	for(State in FSM["states"]){
		if(FSM["states"].hasOwnProperty(State))	len++;
	}
	Angle=(2*Math.PI)/len;
	R=Math.sqrt((R*R)/(2-2*Math.cos((2*Math.PI)/(len+1))));
	CircleRadius=R;
	Xc=R;
	Yc=R;
	i=0;
	document.getElementById("States").innerHTML='<div id="States_Animate" style="display:none;"></div>';
	for(State in FSM["states"]){
		document.getElementById("States").innerHTML+='<div class="state" id="FSM_State.'+State+'" style="top:'+(Yc-Math.sin(Angle*i)*R)+'px;left:'+(Xc+Math.cos(Angle*i)*R)+'px;"><span>'+State+'</span></div>';
		i++;
	}
	for(State in FSM["states"]){
		for(NextState in FSM["states"][State]["nextstates"]){
			if(NextState==State)	continue;
			ArrowName=(State >= NextState)?State+"."+NextState:NextState+"."+State;
			ArrowName="FSM_Arrow."+ArrowName;
			if(document.getElementById(ArrowName)){
				document.getElementById(ArrowName).getElementsByClassName("state_arrowleft")[0].style.display="block";
				document.getElementById(ArrowName).getElementsByClassName("state_arrowright")[0].style.display="block";
				continue;
			}
			PA={x: parseInt(document.getElementById("FSM_State."+State).style.left)+50, y: parseInt(document.getElementById("FSM_State."+State).style.top)+50};
			PB={x: parseInt(document.getElementById("FSM_State."+NextState).style.left)+50, y: parseInt(document.getElementById("FSM_State."+NextState).style.top)+50};
			Vector={x: PA.x-PB.x, y: PA.y-PB.y};
			Topper={x: (PA.x<=PB.x)?PA.x:PB.x, y: (PA.y<=PB.y)?PA.y:PB.y };
			Len={x: Math.abs(Vector.x), y: Math.abs(Vector.y) };
			Size=Math.sqrt(Vector.x*Vector.x+Vector.y*Vector.y);
			document.getElementById("States").innerHTML+='<div class="state_arrow" id="'+ArrowName+'" style="width:'+Len.x+'px;height:'+Len.y+'px;top:'+Topper.y+'px;left:'+Topper.x+'px;"><div class="state_arrowdiv" style="transform:rotate('+Math.atan2(Vector.y,Vector.x)+'rad);margin-left:-'+Size/2+'px;width:'+Size+'px;"><div class="state_arrowleft" style="display:block;">&#9664;</div><div class="state_arrowright" style="display:none;">&#9654;</div></div></div>';
		}
	}
	FSM["currentstate"]=FSM["resetstate"]=document.querySelector('input[name="reset_state"]:checked').value;
	DefineInputs();
	MouseScroll("#States .state");
	document.getElementById("States").style.width=document.getElementById("States").style.height=(2*R+110)+"px";
	Simulation=true;
	$("#DeployMachine").stop().fadeIn(500);
	UpdateView();
}

function SetNextStatesFor(State){
	var i;
	document.getElementById("ConfigureBox_NC").style.display="none";
	document.getElementById("ConfigureBox_WC").style.display="none";
	IsConditionedState=true;
	ConfigState=State;
	document.getElementById("ConfigureBox_WC_Message").innerHTML="Quais serão os próximos estados?";
	document.getElementById("ConfigureBox_WC_Placeholder").placeholder="Próximo estado...";
	document.getElementById("Form_WC_Configs").innerHTML="";
	for(NextState in FSM["states"][State]["nextstates"]){
		document.getElementById("Form_WC_Configs").innerHTML+='<span>Se <input type="text" placeholder="Condição (Verilog)" pattern="^[a-zA-Z0-9_&|~^\\s]+$" value="'+FSM["states"][State]["nextstates"][NextState]+'" title="Uma equação Verilog válida." required>, o próximo estado será <input type="text" list="FSM_Datalist_States" placeholder="Estado" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." value="'+NextState+'" required><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a></span>';
	}
	$("#ConfigureBox").stop().fadeIn(500);
	$("#ConfigureBox_WC").stop().slideDown(1000);
}

function SetOutputsFor(State){
	document.getElementById("ConfigureBox_NC").style.display="none";
	document.getElementById("ConfigureBox_WC").style.display="none";
	ConfigState=State;
	if(FSM["moore"]==true){
		document.getElementById("Form_NC_Configs").innerHTML="";
		for(Output in FSM["states"][State]["outputs"]){
			document.getElementById("Form_NC_Configs").innerHTML+='<span><input type="text" list="FSM_Datalist_Outputs" placeholder="Saída" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." value="'+Output+'" required> será <select><option value="0" '+((FSM["states"][State]["outputs"][Output]=="0")?"selected":"")+'>0</option><option value="1" '+((FSM["states"][State]["outputs"][Output]=="0")?"":"selected")+'>1</option></select><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a></span>';
		}
		$("#ConfigureBox").stop().fadeIn(500);
		$("#ConfigureBox_NC").stop().slideDown(1000);
	}else{
		IsConditionedState=false;
		document.getElementById("ConfigureBox_WC_Message").innerHTML="Quais as saídas desse estado?";
		document.getElementById("ConfigureBox_WC_Placeholder").placeholder="Saída...";
		document.getElementById("Form_WC_Configs").innerHTML="";
		for(Output in FSM["states"][State]["outputs"]){
			for(OutputValue in FSM["states"][State]["outputs"][Output]){
				document.getElementById("Form_WC_Configs").innerHTML+='<span>Se <input type="text" placeholder="Condição (Verilog)" pattern="^[a-zA-Z0-9_&|~^\\s]+$" title="Uma equação Verilog válida." value="'+FSM["states"][State]["outputs"][Output][OutputValue]+'" required>, <input type="text" list="FSM_Datalist_Outputs" placeholder="Saída" pattern="^[a-zA-Z0-9_]+$" title="Apenas caracteres alfanuméricos, sem acentuação." value="'+Output+'" required> será <select><option value="0" '+((OutputValue=="0")?"selected":"")+'>0</option><option value="1" '+((OutputValue=="0")?"":"selected")+'>1</option></select><a href="javascript:void(0);" onclick="MoveMeUp(this);">&uarr;</a><a href="javascript:void(0);" onclick="MoveMeDown(this);">&darr;</a><a href="javascript:void(0);" onclick="DeleteMe(this);">x</a></span>';
			}
		}
		$("#ConfigureBox").stop().fadeIn(500);
		$("#ConfigureBox_WC").stop().slideDown(1000);
	}
}

function ValidateConditionEq(Eq){
	for(Input in FSM["inputs"]){
		Eq=Eq.replace(new RegExp("([^a-zA-Z0-9])("+Input+")([^a-zA-Z0-9])|^("+Input+")([^a-zA-Z0-9])|([^a-zA-Z0-9])("+Input+")$|^("+Input+")$","g"),"$1$6.$3$5");
	}
	Eq=Eq.replace(/[\^][\~]/g,"*").replace(/[\~][\^]/g,"*").replace(/\s/g,"");
	while(Eq.match(/[\(]([~]*[\.])([\&\|\^\*][~]*[\.])*[\)]/g)){
		Eq=Eq.replace(/[\(]([~]*[\.])([\&\|\^\*][~]*[\.])*[\)]/g,".");
	}
	if(Eq.match(/^([~]*[\.])([\&\|\^\*][~]*[\.])*$/g)){
		return true;
	}
	return false;
}
function CalculateConditionEq(Eq){
	for(Input in FSM["inputs"]){
		Eq=Eq.replace(new RegExp("([^a-zA-Z0-9])("+Input+")([^a-zA-Z0-9])|^("+Input+")([^a-zA-Z0-9])|([^a-zA-Z0-9])("+Input+")$|^("+Input+")$","g"),"$1$6"+FSM["inputs"][Input]+"$3$5");
	}
	Eq=Eq.replace(/[\^][\~]/g,"*").replace(/[\~][\^]/g,"*").replace(/\s/g,"");
	while(!Eq.match(/^[01]$/g)){
		if(Eq.match(/~0/g))
			Eq=Eq.replace(/~0/g,"1");
		else if(Eq.match(/~1/g))
			Eq=Eq.replace(/~1/g,"0");
		else if(Eq.match(/0&1|1&0|0&0|0\|0|0\^0|1\^1|0\*1|1\*0/g))
			Eq=Eq.replace(/0&1|1&0|0&0|0\|0|0\^0|1\^1|0\*1|1\*0/g,"0");
		else if(Eq.match(/1&1|1\|0|0\|1|1\|1|1\^0|0\^1|0\*0|1\*1/g))
			Eq=Eq.replace(/1&1|1\|0|0\|1|1\|1|1\^0|0\^1|0\*0|1\*1/g,"1");
		else /*if(Eq.match(/1&1|1\|0|0\|1|1\|1|1\^0|0\^1|0\*0|1\*1/g))*/
			Eq=Eq.replace(/[\(]([01])[\)]/g,"$1");
	}
	return Eq;
}

function DefineInputs(){
	for(Input in FSM["inputs"]){
		if(document.getElementById("InputValue."+Input).checked==true){
			FSM["inputs"][Input]="1";
		}else{
			FSM["inputs"][Input]="0";
		}
	}
	CalculateOutputs();
}
function CalculateOutputs(){
	document.getElementById("CurrentState_Bar").innerHTML=FSM["currentstate"];
	if(FSM["moore"]==true){
		for(Output in FSM["states"][FSM["currentstate"]]["outputs"]){
			document.getElementById("OutputValue."+Output).innerHTML=FSM["states"][FSM["currentstate"]]["outputs"][Output];
		}
	}else{
		for(Output in FSM["states"][FSM["currentstate"]]["outputs"]){
			for(OutputValue in FSM["states"][FSM["currentstate"]]["outputs"][Output]){
				if(CalculateConditionEq(FSM["states"][FSM["currentstate"]]["outputs"][Output][OutputValue])=="1"){
					document.getElementById("OutputValue."+Output).innerHTML=OutputValue;
					break;
				}
			}
		}
	}
}
function Clock(){
	var CurrentState=FSM["currentstate"];
	for(NextState in FSM["states"][CurrentState]["nextstates"]){
		if(CalculateConditionEq(FSM["states"][CurrentState]["nextstates"][NextState])=="1"){
			FSM["currentstate"]=NextState;
			DefineInputs();
			$("#States_Panel").stop().animate({
				scrollLeft:Math.floor(document.getElementById("FSM_State."+FSM["currentstate"]).offsetLeft/document.getElementById("States_Panel").offsetWidth)*document.getElementById("States_Panel").offsetWidth+(document.getElementById("FSM_State."+FSM["currentstate"]).offsetLeft%document.getElementById("States_Panel").offsetWidth),
				scrollTop:Math.floor(document.getElementById("FSM_State."+FSM["currentstate"]).offsetTop/document.getElementById("States_Panel").offsetHeight)*document.getElementById("States_Panel").offsetHeight+(document.getElementById("FSM_State."+FSM["currentstate"]).offsetTop%document.getElementById("States_Panel").offsetHeight)
			}, 2500);
			$(document.getElementById("FSM_State."+CurrentState)).finish().finish().animate({backgroundColor: "rgb(255,255,150)"},1250, function(){
				$(this).animate({backgroundColor: "rgb(255,255,255)"},1250);
			});
			$(document.getElementById("FSM_State."+NextState)).finish().finish().animate({backgroundColor: "rgb(255,255,150)"},1250, function(){
				$(this).animate({backgroundColor: "rgb(255,255,255)"},1250);
			});
			$("#States_Animate").finish();
			document.getElementById("States_Animate").style.top=document.getElementById("FSM_State."+CurrentState).style.top;
			document.getElementById("States_Animate").style.left=document.getElementById("FSM_State."+CurrentState).style.left;
			document.getElementById("States_Animate").style.display="block";
			$("#States_Animate").animate({
				top:document.getElementById("FSM_State."+NextState).style.top,
				left:document.getElementById("FSM_State."+NextState).style.left
			},2500, function(){
				document.getElementById("States_Animate").style.display="none";
			});
			break;
		}
	}
}
function ResetClock(){
	for(Input in FSM["inputs"]){
		FSM["inputs"][Input]="0";
		document.getElementById("InputValue."+Input).checked=false;
	}
	FSM["currentstate"]=FSM["resetstate"];
	DefineInputs();
	UpdateView();
}

function UpdateView(){
	var A, B;
	if(Simulation===true){
		document.getElementById("States").style.marginLeft=document.getElementById("States").style.marginTop=(-1*(CircleRadius+55))+"px";
		if(document.getElementById("DefaultSimulationVision").checked==true){
			A=(window.innerWidth<=window.innerHeight) ? window.innerWidth : window.innerHeight;
			document.getElementById("States_Panel").scrollTop=0;
			document.getElementById("States_Panel").scrollLeft=0;
			if(A<2*CircleRadius+250){
				document.getElementById("States").style.transform="scale("+(A/(2*CircleRadius+250))+")";
			}else{
				document.getElementById("States").style.transform="scale(1.0)";
			}
		}else{
			if(document.getElementById("States").offsetWidth>=document.getElementById("States_Panel").offsetWidth){
				document.getElementById("States").style.marginLeft="0px";
			}
			if(document.getElementById("States").offsetHeight>=document.getElementById("States_Panel").offsetHeight){
				document.getElementById("States").style.marginTop="0px";
			}
			document.getElementById("States").style.transform="scale(1.0)";
			$("#States_Panel").stop().animate({
				scrollLeft:Math.floor(document.getElementById("FSM_State."+FSM["currentstate"]).offsetLeft/document.getElementById("States_Panel").offsetWidth)*document.getElementById("States_Panel").offsetWidth+(document.getElementById("FSM_State."+FSM["currentstate"]).offsetLeft%document.getElementById("States_Panel").offsetWidth),
				scrollTop:Math.floor(document.getElementById("FSM_State."+FSM["currentstate"]).offsetTop/document.getElementById("States_Panel").offsetHeight)*document.getElementById("States_Panel").offsetHeight+(document.getElementById("FSM_State."+FSM["currentstate"]).offsetTop%document.getElementById("States_Panel").offsetHeight)
			}, 250);
		}
	}
}

function SaveNCConfigs(){
	var i, Cmb={ };
	document.getElementById("ConfigureBox_NC").style.display="none";
	NConfigs=document.getElementById("Form_NC_Configs").getElementsByTagName("span");
	for(i=0;i<NConfigs.length;i++){
		if(NConfigs[i].firstElementChild.value=="" || !FSM["outputs"].hasOwnProperty(NConfigs[i].firstElementChild.value) || Cmb.hasOwnProperty(NConfigs[i].firstElementChild.value)){
			document.getElementById("ConfigureBox_NC").style.display="block";
			AlertBox("Você entrou com uma saída inválida (não declarada), ou saídas repetidas!", BreakEvent);
			return;
		}
		Cmb[NConfigs[i].firstElementChild.value]=NConfigs[i].getElementsByTagName("select")[0].value;
	}
	FSM["states"][ConfigState]["outputs"]=Cmb;
	document.getElementById("ConfigureBox_NC").style.display="block";
	$("#ConfigureBox_NC").slideUp(500);
	$("#ConfigureBox").fadeOut(1000);
}
function SaveWCConfigs(){
	var i, Cmb={ }, AuxCmb={ };
	document.getElementById("ConfigureBox_WC").style.display="none";
	NConfigs=document.getElementById("Form_WC_Configs").getElementsByTagName("span");
	if(IsConditionedState){ // Estados
		for(i=0;i<NConfigs.length;i++){
			if(NConfigs[i].firstElementChild.value=="" || !ValidateConditionEq(NConfigs[i].firstElementChild.value)){
				document.getElementById("ConfigureBox_WC").style.display="block";
				AlertBox("Equação Verilog não válida!", BreakEvent);
				return;
			}
			if(!FSM["states"].hasOwnProperty(NConfigs[i].getElementsByTagName("input")[1].value)){
				document.getElementById("ConfigureBox_WC").style.display="block";
				AlertBox("Você entrou com um estado inválido (não declarado)!", BreakEvent);
				return;
			}
			if(Cmb.hasOwnProperty(NConfigs[i].getElementsByTagName("input")[1].value)){
				document.getElementById("ConfigureBox_WC").style.display="block";
				AlertBox("Você designou mais de uma equação Verilog para o mesmo estado!", BreakEvent);
				return;
			}
			Cmb[NConfigs[i].getElementsByTagName("input")[1].value]=NConfigs[i].firstElementChild.value;
			AuxCmb[NConfigs[i].getElementsByTagName("input")[1].value]=NConfigs[i].firstElementChild.value;
		}
		FSM["states"][ConfigState]["nextstates"]=AuxCmb;
	}else{ // Saídas Mealy
		for(i=0;i<NConfigs.length;i++){
			if(NConfigs[i].firstElementChild.value=="" || !ValidateConditionEq(NConfigs[i].firstElementChild.value)){
				document.getElementById("ConfigureBox_WC").style.display="block";
				AlertBox("Equação Verilog inválida!", BreakEvent);
				return;
			}
			if(!FSM["outputs"].hasOwnProperty(NConfigs[i].getElementsByTagName("input")[1].value)){
				document.getElementById("ConfigureBox_WC").style.display="block";
				AlertBox("Você entrou com uma saída inválida (não declarada)!", BreakEvent);
				return;
			}
			if(Cmb.hasOwnProperty(NConfigs[i].getElementsByTagName("input")[1].value+"."+NConfigs[i].getElementsByTagName("select")[0].value)){
				document.getElementById("ConfigureBox_WC").style.display="block";
				AlertBox("Você designou mais de uma equação Verilog para a mesma saída!", BreakEvent);
				return;
			}
			if(!AuxCmb.hasOwnProperty(NConfigs[i].getElementsByTagName("input")[1].value)){
				AuxCmb[NConfigs[i].getElementsByTagName("input")[1].value] = { };
			}
			Cmb[NConfigs[i].getElementsByTagName("input")[1].value+"."+NConfigs[i].getElementsByTagName("select")[0].value]=NConfigs[i].firstElementChild.value;
			AuxCmb[NConfigs[i].getElementsByTagName("input")[1].value][NConfigs[i].getElementsByTagName("select")[0].value]=NConfigs[i].firstElementChild.value;
		}
		FSM["states"][ConfigState]["outputs"]=AuxCmb;
	}
	document.getElementById("ConfigureBox_WC").style.display="block";
	$("#ConfigureBox_WC").slideUp(500);
	$("#ConfigureBox").fadeOut(1000);
}
function CancelNCConfigs(){
	$("#ConfigureBox_NC").slideUp(500);
	$("#ConfigureBox").fadeOut(1000);
}
function CancelWCConfigs(){
	$("#ConfigureBox_WC").slideUp(500);
	$("#ConfigureBox").fadeOut(1000);
}

function BackToMachineDescription(){
	document.getElementById("DevelopMachineArchitecture").style.display="none";
	$("#DevelopMachine").stop().fadeIn(500);
}

function BackToMachineArchitecture(){
	document.getElementById("DeployMachine").style.display="none";
	Simulation=false;
	$("#DevelopMachineArchitecture").stop().fadeIn(500);
}

function MouseScroll(Elem){
	$(Elem).mouseenter(function(){
		$(this).stop().animate({scrollLeft: this.scrollWidth}, this.scrollWidth*10, "linear");
	}).mouseleave(function(){
		$(this).stop().animate({scrollLeft: 0}, 250);
	});
}
function AlertBox(Message, OnClose){
	document.getElementById("MessageAlertBox").innerHTML=Message;
	MessageBox.onok=OnClose;
	document.getElementById("ConfirmBox").style.display="none";
	document.getElementById("AlertBox").style.display="none";
	$("#MessageBox").stop().fadeIn(500);
	$("#AlertBox").stop().slideDown(1000);
	$("#AlertBox .messageBox_buttons a:first-child").focus();
}
function ConfirmBox(Message, OnYes, OnNo){
	document.getElementById("MessageConfirmBox").innerHTML=Message;
	MessageBox.onyes=OnYes;
	MessageBox.onno=OnNo;
	document.getElementById("AlertBox").style.display="none";
	document.getElementById("ConfirmBox").style.display="none";
	$("#MessageBox").stop().fadeIn(500);
	$("#ConfirmBox").stop().slideDown(1000);
	$("#ConfirmBox .messageBox_buttons a:nth-child(2)").focus();
}


function WindowExit(){
	if(ExitConfirmation===true){
		return ExitConfirmationMessage;
	}
}

function ToggleFixed(El, Fix){
	var Me=document.getElementById(El);
	if(Fix==true){
		Me.classList.add("fixed");
	}else{
		Me.classList.remove("fixed");
	}
}

function MainFSM(){
	document.getElementById("DevelopMachine").ondragover=ParseDraggedFile;
	document.getElementById("DevelopMachine").ondrop=ParseDroppedFile;
	document.getElementById("DevelopMachine").style.display="block";
	MouseScroll(CurrentState_Bar);
	$("#SplashScreen").stop().fadeOut(500);
}
function BreakEvent(){
	return false;
}