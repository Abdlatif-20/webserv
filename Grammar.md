<main> = "server" "{" {<server_directives> | <location>+} "}"

<location> = "location" <WORD> "{" {<location_directives>} "}"

<server_directives> = {<server_directive> ";"}
<location_directives> = {<location_directive> ";"}
<http_directives> = {<http_directive> ";"}
<http_directive> = "root" <WORD>
                    | "index" <WORD>+
                    | "auto_index" {"on" OR "off"}
                    | "error_page" <STATUS_CODE> <WORD>
                    | "client_max_body_size" <WORD> <!-- To be checked later, example: 20MB, 2GB -->
                    | "allowed_methods" {"GET" | "POST" | "DELETE"}
                    | "upload_store" <WORD>

<server_directive> = "listen" <WORD>
                    | "server_name" <WORD>+
                    | "return" <INT> <WORD>
                    | <http_directive>

<location_directive> = <http_directive>
                    | "return" <INT> <WORD>