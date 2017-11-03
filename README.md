Fish AmpMod
===========

Fish AmpMod is a customizable LV2 amplifier plugin intended to be used with
automation. Its distinguishing feature is the ability to control exactly how
the amplitude changes. For example, as the amplification is decreased, does the
amplitude quickly decay, slowly decay, or move linearly? All of these settings,
and more, can be configured.


Usage
-----

“Exponent” and “Amplitude base” are the main controls that affect the
waveform’s amplitude. At each point in time, a multiplier for the amplitude
is calculated with the following formula:

```
Multiplier = (Amplitude base) ^ Exponent
```

Each sample in the waveform is then multiplied by the multiplier.

“Amplitude base” is the control intended to be automated. When set to 0, the
multiplier is 0, and when set to 1, the multiplier is 1.

When the exponent is 1, the base linearly controls the multiplier. When the
exponent is small, the multiplier will first quickly increase as the base moves
from 0 to 1 and then slowly approach the maximum multiplier when closer to 1.
And when the exponent is large, the multiplier will slowly increase at first
and then quickly increase when the base approaches 1.

“Increment/ms” controls the amount of smoothing applied when the base
changes. When the base changes, the current base will increase or decrease over
time in the direction of the new base. The “Increment/ms” control sets how
much the base can change per millisecond. If it is set to 0, then no smoothing
is applied and a change in the base will have an immediate effect (this is
essentially an increment of infinity).

“Minimum multiplier” controls the multiplier’s value when the base is 0. By
default, the minimum multiplier is 0 (so the multiplier and amplitude become 0
when the base is 0). The multiplier formula used when the minimum multiplier is
not 0 is:

```
Multiplier =
  (1 - (Minimum multiplier)) *
  ((Amplitude base) ^ Exponent) +
  (Minimum multiplier)
```


Dependencies
------------

* LV2 development files
* GCC
* GNU Make

On Debian GNU/Linux (and many derivatives), these can be installed by running
the following command as root:

```
apt-get install lv2-dev gcc make
```


Installation
------------

Run the following commands (you will need to have [Git] installed):

```
git clone https://github.com/taylordotfish/ampmod ~/.lv2/fish-ampmod.lv2/
cd ~/.lv2/fish-ampmod.lv2/
make
```

[Git]: https://git-scm.com/


License
-------

Fish AmpMod is licensed under the GNU General Public License, version 3 or
any later version. See [LICENSE].

This README file has been released to the public domain using [CC0].

[LICENSE]: LICENSE
[CC0]: https://creativecommons.org/publicdomain/zero/1.0/
