"use strict";

var element = document.querySelector('#mybtn');


function add(target)
{
	let body = `<!DOCTYPE html>
	<html lang="en">
	<head>
		<script src="code.js"></script>
		<link rel="stylesheet" href="webserv.css">
		<link rel="preconnect" href="https://fonts.googleapis.com">
		<link href="https://fonts.googleapis.com/css2?family=Pacifico&display=swap" rel="stylesheet">
		<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
		<link href="https://fonts.googleapis.com/css2?family=Cutive+Mono&display=swap" rel="stylesheet">
		<meta charset="UTF-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Webserv</title>
	</head>
	<body>
		<div class="header">
			<div class="content">
				<button class="previous" onclick="go_to('index.html')"></button>
				<p>&lt;Webserv&gt;</p><p style class="method">delete_method</p>
				
			</div>
		</div>
		<div class="delete_text">
			<h1>File to delete</h1>
			<p>This file must be deleted by the delete method.</p>
			<!-- <p>Well done for executing a valid request with our webserv !</p> -->
		</div>
	</body>
	</html>`;
	const xhr = new XMLHttpRequest();
	xhr.open("POST", target);
	xhr.send(body);
}

function go_to(arg1)
{
	window.location = arg1;
}



function remove(target)
{
	if (target && target != "/")
	{
		const xhr = new XMLHttpRequest();
		xhr.open("DELETE", target);
		xhr.send(null);
	}
}

function get(arg1)
{
	let target;

	if (arg1 == "valid") {
		target = "http://localhost:4242/valid_request.html";
	} else if (arg1 == "invalid") {
		target = "http://localhost:4242/valid_reque";
	} else {
		target = prompt("Please enter the target of the request: ", '');
	}
	
	const xhr = new XMLHttpRequest();
	xhr.open("GET", target);
	xhr.send(null);
	xhr.onreadystatechange = function() {
		if(xhr.readyState==4) {
			console.log(`request: GET ${target} HTTP/1.1`);
			console.log(`response:  HTTP/1.1 ${xhr.status} ${xhr.statusText}\n ${xhr.responseText} ${xhr.getAllResponseHeaders()}`);
		}
	}
	if (confirm("1. Accept if you want to follow the url.\n2. Cancel and open the console to see the logs.") == true) {
		window.location = target;
	}
	
}

function post(arg1)
{
	let target;
	let body;

	body = `<!DOCTYPE html>
		<html lang="en">
		<head>
			<script src="code.js"></script>
			<link rel="stylesheet" href="webserv.css">
			<link rel="preconnect" href="https://fonts.googleapis.com">
			<link href="https://fonts.googleapis.com/css2?family=Pacifico&display=swap" rel="stylesheet">
			<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
			<link href="https://fonts.googleapis.com/css2?family=Cutive+Mono&display=swap" rel="stylesheet">
			<meta charset="UTF-8">
			<meta http-equiv="X-UA-Compatible" content="IE=edge">
			<meta name="viewport" content="width=device-width, initial-scale=1.0">
			<title>Webserv</title>
		</head>
		<body>
			<div class="header">
				<div class="content">
					<button class="previous" onclick="go_to('index.html')"></button>
					<p>&lt;Webserv&gt;</p><p style class="method">post_method</p>
					
				</div>
			</div>
			<div class="valid_text">
				<h1>Valid request!</h1>
				<p>To see the response and request content, see the logs in the console of the previous page.</p>
				<!-- <p>Well done for executing a valid request with our webserv !</p> -->
			</div>
		</body>
		</html>\0`;
	if (arg1 == "valid") {
		target = "http://localhost:4242/new.html";
		
	} else if (arg1 == "invalid") {
		target = "http://localhost:4242/tmp.html";
	} else {
		target = "http://localhost:4242/new.html";
		body = prompt("Please enter the body of the request: ", '');
	}
	if (confirm("Before sending the request make sure that new.html doesn't exist.") == true)
	{
	const xhr = new XMLHttpRequest();
	xhr.open("POST", target);
	xhr.onreadystatechange = function() {
		if(xhr.readyState==4) {
			console.log(`request: POST ${target} HTTP/1.1`);
			console.log(`response:  HTTP/1.1 ${xhr.status} ${xhr.statusText}\n ${xhr.responseText}`);
		}
	}
	xhr.send(body);
	if (confirm("1. Accept if you want to follow the url.\n2. Cancel and open the console to see the logs.") == true) {
		window.location = target;
	}
}
	
}

function delet(arg1)
{
	let target;

	if (arg1 == "valid") {
		target = "http://localhost:4242/to_delete.html";
	} else if (arg1 == "invalid") {
		target = "http://localhost:4242/to_del";
	} else {
		target = prompt("Please enter the target of the request: ", '');
	}
	if (confirm("Before sending the request make sure that to_delete.html exist.") == true)
	{
	const xhr = new XMLHttpRequest();
	xhr.open("DELETE", target);
	xhr.onreadystatechange = function() {
		if(xhr.readyState==4) {
			console.log(`request: DELETE ${target} HTTP/1.1`);
			console.log(`response:  HTTP/1.1 ${xhr.status} ${xhr.statusText}\n ${xhr.responseText}`);
		}
	}
	xhr.send(null);
	}
}

function requestCalculator(){
	var inputX = document.getElementById("inputX").value;
	var inputY = document.getElementById("inputY").value;
	const inputOperator = document.getElementById("operator").value;
	
	const target = "http://localhost:4242/calculator.html?x=" + inputX + "&y=" + inputY + "&operator=" + inputOperator;
	
	const xhr = new XMLHttpRequest();
	xhr.open("GET", target);
	xhr.send(null);
	xhr.onreadystatechange = function() {
		if(xhr.readyState==4) {
			console.log(`request: GET ${target} HTTP/1.1`);
			console.log(`response:  HTTP/1.1 ${xhr.status} ${xhr.statusText}\n ${xhr.responseText} ${xhr.getAllResponseHeaders()}`);
			window.location = "http://localhost:4242/calculator.html";
		}
	}
	// document.getElementsByClassName('s')[0].style
	// document.getElementsByClassName('result')[0].style.visibility = "visible";

}

function hideResult(){
	document.getElementsByClassName('result')[0].style.visibility = "hidden";
}