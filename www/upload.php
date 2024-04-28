<?php
$targetDir = "Upload/";
$targetFile = $targetDir . $_FILES["file"]["name"];
if (move_uploaded_file($_FILES["file"]["tmp_name"], $targetFile))
{
  http_response_code(201);
  echo "<!DOCTYPE html>
  <html>
    <head>
      <title>Created</title>
    </head>
    <body>
        <div id='created_div'>
            <h1>201 - Created</h1>
        </div>
    </body>
  </html>
  ";
}
?>