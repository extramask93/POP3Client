# POP3Client
clone with:<br />
git clone --recursive https://github.com/extramask93/POP3Client.git<br />
as the repo contains submodule with socket library.<br />
To build you would need c++17 compliant compiler<br />
Configuration file should be named config.ini and should be placed within the same folder as executable.<br />
Minimum configuration:<br />
ip = pop3_service_ip<br />
login = your_login<br />
pass = your_password<br />
port = port_of_pop3_server<br />
delay = delay_between_subsequent_reads_in_seconds_integer<br />
