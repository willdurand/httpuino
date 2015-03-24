httpuino
========

[![Build
Status](https://travis-ci.org/willdurand/httpuino.svg?branch=master)](https://travis-ci.org/willdurand/httpuino)

Yet another thin HTTP + HTCPCP message parser written in C, with a small memory
footprint. It basically transforms any HTTP message into a C structure containing
an HTTP _method_ (verb), a _path_, a set of _headers_, and a _body_.

**Note:** This project was initially part of the
[coffuino](https://github.com/willdurand/coffuino) project, but has moved to its
own repository because it can be widely used.


Usage
-----

Please see
[`src/httpuino.h`](https://github.com/willdurand/httpuino/blob/master/src/httpuino.h).


Unit Tests
----------

    cd test/ ; make test


License
-------

httpuino is released under the MIT License. See the bundled LICENSE file for
details.
