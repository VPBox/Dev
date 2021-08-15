The filenames in the cacerts directory are in the format of <hash>.<n>
where "hash" is the subject hash produced by:

    openssl x509 -subject_hash_old -in filename

and the "n" is a unique integer identifier starting at 0 to deal
with collisions. See OpenSSL's c_rehash manpage for details.
