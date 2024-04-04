<?php
// header("Status: 200");
// echo "GATEWAY_INTERFACE : ".getenv("GATEWAY_INTERFACE")."</br>\n";
// echo "SERVER_SOFTWARE : ".getenv("SERVER_SOFTWARE")."</br>\n";
// echo "SERVER_PROTOCOL : ".getenv("SERVER_PROTOCOL")."</br>\n";
// echo "SERVER_PORT : ".getenv("SERVER_PORT")."</br>\n";
// echo "REQUEST_METHOD : ".getenv("REQUEST_METHOD")."</br>\n";
// echo "PATH_INFO : ".getenv("PATH_INFO")."</br>\n";
// echo "SCRIPT_NAME : ".getenv("SCRIPT_NAME")."</br>\n";
// echo "DOCUMENT_ROOT : ".getenv("DOCUMENT_ROOT")."</br>\n";
// echo "QUERY_STRING : ".getenv("QUERY_STRING")."</br>\n";
// echo "CONTENT_TYPE : ".getenv("CONTENT_TYPE")."</br>\n";
// echo "CONTENT_LENGTH : ".getenv("CONTENT_LENGTH")."</br>\n";
// echo "REDIRECT_STATUS : ".getenv("REDIRECT_STATUS")."</br>\n";
// echo "PATH : ".getenv("PATH")."</br>\n";
// phpinfo();
if(isset($_COOKIE["user"]))
{
	setcookie("user", $_POST["user"], time()+3600);
	echo "here => ".$_COOKIE["user"];
}
else
	echo "not setting"

?>