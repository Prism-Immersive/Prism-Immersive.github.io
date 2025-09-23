# Cloud Rendering Server Library

### Run on WSL

To run on WSL, first forward the ports 8890~8893 of Windows to WSL, then open the firewall settings for them.
To forward, run
```shell
# Repeat this for ports 8891~8893 as well
netsh interface portproxy add v4tov4 listenport=8890 listenaddress=0.0.0.0 connectport=8890 connectaddress=<IP ADDR>
```
where `<IP ADDR>` is the IP of WSL found via `ifconfig` in WSL.

To open firewall, see [here](https://learn.microsoft.com/en-us/sql/reporting-services/report-server/configure-a-firewall-for-report-server-access?view=sql-server-ver16).