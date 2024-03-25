<?php
echo "GATEWAY_INTERFACE : ".getenv("GATEWAY_INTERFACE")."</br>";
echo "SERVER_NAME : ".getenv("SERVER_NAME")."</br>";
echo "SERVER_SOFTWARE : ".getenv("SERVER_SOFTWARE")."</br>";
echo "SERVER_PROTOCOL : ".getenv("SERVER_PROTOCOL")."</br>";
echo "SERVER_PORT : ".getenv("SERVER_PORT")."</br>";
echo "REQUEST_METHOD : ".getenv("REQUEST_METHOD")."</br>";
echo "PATH_INFO : ".getenv("PATH_INFO")."</br>";
echo "PATH_TRANSLATED : ".getenv("PATH_TRANSLATED")."</br>";
echo "SCRIPT_NAME : ".getenv("SCRIPT_NAME")."</br>";
echo "DOCUMENT_ROOT : ".getenv("DOCUMENT_ROOT")."</br>";
echo "QUERY_STRING : ".getenv("QUERY_STRING")."</br>";
echo "REMOTE_HOST : ".getenv("REMOTE_HOST")."</br>";
echo "REMOTE_ADDR : ".getenv("REMOTE_ADDR")."</br>";
echo "AUTH_TYPE : ".getenv("AUTH_TYPE")."</br>";
echo "REMOTE_USER : ".getenv("REMOTE_USER")."</br>";
echo "REMOTE_IDENT : ".getenv("REMOTE_IDENT")."</br>";
echo "CONTENT_TYPE : ".getenv("CONTENT_TYPE")."</br>";
echo "CONTENT_LENGTH : ".getenv("CONTENT_LENGTH")."</br>";
echo "HTTP_FROM : ".getenv("HTTP_FROM")."</br>";
echo "HTTP_ACCEPT : ".getenv("HTTP_ACCEPT")."</br>";
echo "HTTP_USER_AGENT : ".getenv("HTTP_USER_AGENT")."</br>";
echo "HTTP_REFERER : ".getenv("HTTP_REFERER")."</br>";
phpinfo()

?>