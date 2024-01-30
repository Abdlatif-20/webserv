<main> = "server" "{" {<server_directives> | <location>+} "}"

<location> = "location" <WORD> "{" {<location_directives>} "}"

<server_directives> = {<server_directive> ";"}
<location_directives> = {<location_directive> ";"}
<http_directives> = {<http_directive> ";"}

<http_directive> = "root" <WORD>
                    | "index" <WORD>+
                    | "error_page" <STATUS_CODE> <WORD>
                    | "client_max_body_size" <WORD> <!-- To be checked later, example: 20MB, 2GB -->
                    | "allowed_methods" {"GET" | "POST" | "DELETE"}

<server_directive> = "listen" <WORD>
                    | <http_directive>
                    | "server_name" <WORD>+
                    | "return" <INT> <WORD>

<location_directive> = <http_directive>
                    | "return" <INT> <WORD>