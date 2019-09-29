# atst_aa_rev01a

testing-ee current.  (Sunday, 29 July at 02:20 UTC)

This branch (testing-ee) has a functional Pimoroni Blinkt
demonstration program.  The Blinkt uses APA102 ('dotstar')
RGB led technology.

See: http://adafru.it/3341 - APA102
See: http://adafru.it/3195 - Pimoroni Blinkt!

Technique used is to bitbang the Blinkt, using GPIO, just as
one would for a shift register.

Simple.  And it works.

older news:

Branch master has a functioning LED blinkie.

Moving forward, branch testing-dd is retired.  New program
development will be on branch testing-ee.

Current program is good for the Adafruit Feather M0 Express,
their CPX and Metro M0 Express (most likely will function
with any ATSAMD21G18-AU board that has a traditional D13 LED
mapped onto PA17).

Sun Jul 29 02:19:44 UTC 2018
