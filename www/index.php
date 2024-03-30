<?php
// header("Status: 200");
echo "GATEWAY_INTERFACE : ".getenv("GATEWAY_INTERFACE")."</br>\n";
// echo "SERVER_NAME : ".getenv("SERVER_NAME")."</br>\n";
echo "SERVER_SOFTWARE : ".getenv("SERVER_SOFTWARE")."</br>\n";
echo "SERVER_PROTOCOL : ".getenv("SERVER_PROTOCOL")."</br>\n";
echo "SERVER_PORT : ".getenv("SERVER_PORT")."</br>\n";
echo "REQUEST_METHOD : ".getenv("REQUEST_METHOD")."</br>\n";
echo "PATH_INFO : ".getenv("PATH_INFO")."</br>\n";
// echo "PATH_TRANSLATED : ".getenv("PATH_TRANSLATED")."</br>\n";
echo "SCRIPT_NAME : ".getenv("SCRIPT_NAME")."</br>\n";
echo "DOCUMENT_ROOT : ".getenv("DOCUMENT_ROOT")."</br>\n";
echo "QUERY_STRING : ".getenv("QUERY_STRING")."</br>\n";
// echo "REMOTE_HOST : ".getenv("REMOTE_HOST")."</br>\n";
// echo "REMOTE_ADDR : ".getenv("REMOTE_ADDR")."</br>\n";
// echo "AUTH_TYPE : ".getenv("AUTH_TYPE")."</br>\n";
// echo "REMOTE_USER : ".getenv("REMOTE_USER")."</br>\n";
// echo "REMOTE_IDENT : ".getenv("REMOTE_IDENT")."</br>\n";
echo "CONTENT_TYPE : ".getenv("CONTENT_TYPE")."</br>\n";
echo "CONTENT_LENGTH : ".getenv("CONTENT_LENGTH")."</br>\n";
// echo "HTTP_FROM : ".getenv("HTTP_FROM")."</br>\n";
// echo "HTTP_ACCEPT : ".getenv("HTTP_ACCEPT")."</br>\n";
// echo "HTTP_USER_AGENT : ".getenv("HTTP_USER_AGENT")."</br>\n";
// echo "HTTP_REFERER : ".getenv("HTTP_REFERER")."</br>\n";
echo "REDIRECT_STATUS : ".getenv("REDIRECT_STATUS")."</br>\n";
echo "PATH : ".getenv("PATH")."</br>\n";
// phpinfo()
while (1);
echo "hello";
?>