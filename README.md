# pg_nanp
an efficient, validated, formatted NANP type

<h3>Description</h3>
This type supports efficient, validated, formatted usage of North American Numbering Plan (NANP) data. It only accepts valid NANP numeric sequences, rejecting sequences that violate NANP specifications. On the other hand, it supports numerous typical formatting embellishments to plain NANP numeric sequences. It formats output consistently in ###-###-#### format.

<h3>Installation</h3>
<pre>
curl -s -S -L https://github.com/rlichtenwalter/pg_nanp/archive/master.zip > pg_nanp.zip
unzip pg_nanp.zip
(cd pg_nanp-master &amp;&amp; make PG_CONFIG=&lt;optional custom pg_config path&gt;)
(cd pg_nanp-master &amp;&amp; make PG_CONFIG=&lt;optional custom pg_config path&gt; install)
(cd ~postgres &amp;&amp; sudo -u postgres psql -c 'CREATE EXTENSION pg_nanp;')
</pre>

<h3>Usage</h3>
<pre>
testuser=# SELECT '2345678910'::nanp;
     nanp
--------------
 234-567-8910
(1 row)

testuser=# SELECT '12345678910'::nanp;
     nanp
\--------------
 234-567-8910
(1 row)

testuser=# SELECT '22345678910'::nanp;
ERROR:  invalid input format for nanp: '22345678910'
LINE 1: SELECT '22345678910'::nanp;

testuser=# SELECT '1345678910'::nanp;
ERROR:  invalid input format for nanp: '1345678910'
LINE 1: SELECT '1345678910'::nanp;


testuser=# SELECT '1145678910'::nanp;
ERROR:  invalid input format for nanp: '1145678910'
LINE 1: SELECT '1145678910'::nanp;

testuser=# SELECT '2341678910'::nanp;
ERROR:  invalid input format for nanp: '2341678910'
LINE 1: SELECT '2341678910'::nanp;
</pre>
