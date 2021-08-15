#!/usr/bin/env python

import argparse
import policy

parser = argparse.ArgumentParser(
    description="SELinux policy rule search tool. Intended to have a similar "
        + "API as sesearch, but simplified to use only code availabe in AOSP")
parser.add_argument("policy", help="Path to the SELinux policy to search.", nargs="?")
parser.add_argument("--libpath", dest="libpath", help="Path to the libsepolwrap.so", nargs="?")
tertypes = parser.add_argument_group("TE Rule Types")
tertypes.add_argument("--allow", action="append_const",
                    const="allow", dest="tertypes",
                    help="Search allow rules.")
expr = parser.add_argument_group("Expressions")
expr.add_argument("-s", "--source",
                  help="Source type/role of the TE/RBAC rule.")
expr.add_argument("-t", "--target",
                  help="Target type/role of the TE/RBAC rule.")
expr.add_argument("-c", "--class", dest="tclass",
                  help="Comma separated list of object classes")
expr.add_argument("-p", "--perms", metavar="PERMS",
                  help="Comma separated list of permissions.")

args = parser.parse_args()

if not args.tertypes:
    parser.error("Must specify \"--allow\"")

if not args.policy:
    parser.error("Must include path to policy")
if not args.libpath:
    parser.error("Must include path to libsepolwrap library")

if not (args.source or args.target or args.tclass or args.perms):
    parser.error("Must something to filter on, e.g. --source, --target, etc.")

pol = policy.Policy(args.policy, None, args.libpath)

if args.source:
    scontext = {args.source}
else:
    scontext = set()
if args.target:
    tcontext = {args.target}
else:
    tcontext = set()
if args.tclass:
    tclass = set(args.tclass.split(","))
else:
    tclass = set()
if args.perms:
    perms = set(args.perms.split(","))
else:
    perms = set()

TERules = pol.QueryTERule(scontext=scontext,
                       tcontext=tcontext,
                       tclass=tclass,
                       perms=perms)

# format rules for printing
rules = []
for r in TERules:
    if len(r.perms) > 1:
        rules.append("allow " + r.sctx + " " + r.tctx + ":" + r.tclass + " { " +
                " ".join(r.perms) + " };")
    else:
        rules.append("allow " + r.sctx + " " + r.tctx + ":" + r.tclass + " " +
                " ".join(r.perms) + ";")

for r in sorted(rules):
    print r
