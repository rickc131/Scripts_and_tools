import ipaddress
import re 
import sys

ip_pattern = r'^(.+)/(16|24)$'

private_networks = ['10.0.0.0/8', '172.16.0.0/12', '192.168.0.0/16']

def check_ip(ip):
  addr, mask = ip.split('/')
  mask_str = str(mask)

  network = ipaddress.ip_network(addr + '/' + mask_str, strict=False)  

  private_subnets = []
  for priv_net in private_networks:
    priv_net_obj = ipaddress.ip_network(priv_net)
    if network.subnet_of(priv_net_obj):
      private_subnets.append(str(network))

  if private_subnets:
    output = f"{ip} contains subnets: "
    output += '\n'.join(private_subnets)
  else:
    output = f"{ip} does not contain private subnets"

  return output

if len(sys.argv) > 1:

  if sys.argv[1] == '-u':
    ip = sys.argv[2]  
    output = check_ip(ip)

    if '-o' in sys.argv:
      output_file = sys.argv[sys.argv.index('-o') + 1]
      with open(output_file, 'w') as f:
        print(output, file=f)
    else:
      print(output)

  elif sys.argv[1] == '-r':
    with open(sys.argv[2]) as f:
      for line in f:
        ip = line.strip()
        output = check_ip(ip)
        
        if '-o' in sys.argv:  
          output_file = sys.argv[sys.argv.index('-o') + 1]
          with open(output_file, 'a') as f:
            print(output, file=f)
        else:
          print(output)
  
  else:
    print("Usage:")
    print("  -u ip/16")
    print("  -u ip/24")
    print("  -r ip.txt -o output.txt")

else:
  print("Usage:")
  print("  -u ip/16")
  print("  -u ip/24")
  print("  -r ip.txt -o output.txt")
