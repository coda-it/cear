# cear

#### Motivation:
Listen to incomming TCP connections on a port and execute scripts on server when TCP connections is accepted.

#### Installation:
1. Clone repository and build the binary with `make`
2. Add `cear.conf` file within the same directory with the script that needs to be executed when `cear` will detect connection (it should contain a bash script, it can also contain env variables passed by `X-Cear-Params` header)
3. Create a systemd service (ex. `/etc/systemd/system/cear.service`)
4. Put there env variable (ex. by adding the record `Environment="CEAR_KEY=<private key>"`)
5. Restart services `sudo systemctl daemon-reload && sudo systemctl start cear.service`
6. `cear` will be up and running on port `4000`

#### Testing
You can test `cear` by invoking `curl --header "X-Cear-Auth: <private key>" --header "X-Cear-Params: VAR_1=val_1" -X POST <ip>:4000`
