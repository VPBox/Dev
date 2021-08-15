#!/usr/bin/python

"""Extract certificates from a multi-certificate pem file.

Each certificate in the file is extracted into a format appropriate for use with
Brillo or Android. On success, the contents of the output directory match the
input file exactly. Existing files in the output directory will be deleted.

The current date will be written into the timestamp file, './TIMESTAMP' by
default.

Typical usage (extracting from ./roots.pem and output into ./files):
> ./extract_from_pem.py
"""

import argparse
import datetime
import os
import re

import M2Crypto  # sudo apt-get install python-m2crypto


def WriteCertificateFile(content, base_name, output_dir):
  """Writes a certificate file to the output directory.

  Args:
    content: The file content to write.
    base_name: The file name will be base_name.n where n is the first available
               non-negative integer. Ex. if myfile.0 exists and has different
               content, the output file will be myfile.1.
    output_dir: The output directory.
  """
  i = 0
  file_path = os.path.join(output_dir, '%s.%d' % (base_name, i))
  while os.path.exists(file_path):
    with open(file_path) as existing_file:
      if content == existing_file.read():
        # Ignore identical duplicate.
        return
    i += 1
    file_path = os.path.join(output_dir, '%s.%d' % (base_name, i))
  with open(file_path, 'w') as new_file:
    new_file.write(content)


def GetFingerprintString(x509):
  """Computes a fingerprint string as output by 'openssl x509 -fingerprint'.

  Args:
    x509: A M2Crypto.X509.X509 object.

  Returns:
    The fingerprint as a string.
  """
  # Zero filled and with ':' between bytes.
  return ':'.join(re.findall(r'..', x509.get_fingerprint('sha1').zfill(40)))


def main():
  parser = argparse.ArgumentParser(description='PEM Certificate Importer')
  parser.add_argument('--pem_file', nargs='?', default='roots.pem')
  parser.add_argument('--output_dir', nargs='?', default='files')
  parser.add_argument('--timestamp_file', nargs='?', default='TIMESTAMP')
  args = parser.parse_args()
  assert os.path.isdir(args.output_dir) and os.path.isfile(args.pem_file)
  if 'y' != raw_input('All files in \'%s\' will be deleted. Proceed? [y,N]: ' %
                          args.output_dir):
    print 'Aborted.'
    return
  for existing_file in os.listdir(args.output_dir):
    os.remove(os.path.join(args.output_dir, existing_file))
  with open(args.pem_file) as pem_file:
    pattern = r'-----BEGIN CERTIFICATE-----[^-]*-----END CERTIFICATE-----'
    pem_certs = re.findall(pattern, pem_file.read())
    for pem_cert in pem_certs:
      x509 = M2Crypto.X509.load_cert_string(pem_cert)
      content = '%s%sSHA1 Fingerprint=%s\n' % (x509.as_pem(),
                                               x509.as_text(),
                                               GetFingerprintString(x509))
      base_name = '%08x' % x509.get_subject().as_hash()
      WriteCertificateFile(content, base_name, args.output_dir)
  with open(args.timestamp_file, 'w') as timestamp_file:
    timestamp_file.write('Last Update (YYYY-MM-DD): %s\n' %
                             datetime.date.today().isoformat())


if __name__ == '__main__':
  main()
