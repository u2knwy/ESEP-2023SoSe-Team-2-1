# Festo Transfer System Simulation #

Simulation software for the Festo Transfer System at HAW Hamburg.

## Features of Release 22 / Visualisation 13 (2023-02-23)
- adds patch: Remove selected item via GUI.
- internal change of configuration evaluation

## Features of Release 21 (2023-01-12)
- fixes startup bug: if application is faster on startup, it gets no wiered results anymore.
- fixes IRQ bug: if pending interrupts are not handled properly, now ISR is called again/event is sent again.

## Features of Release 20a (2021-12-19)
- Making clear in documentation, that interrupt line 0 for GPIO-Interrupts cannot be used.

## Features of Release 20 (2021-11-30)
- Automatic configuration of system name and kind of feed separator in visualisation (version 9).

## Features of Release 19a (2021-10-14)
- Fixes documentation according communication settings.

## Features of Release 19 (2021-08-11) ##
- Support of non-default start positions.
- Movement along guidance rails
- Tools, except visualisation, support injection at non-default start positions. 

## Features of Release 18e (2021-06-19) ##
- Visualisation Version 8: Selection of properties `Sticky` and `Flip` (contribution by David Berschauer).

## Features of Release 18d (2021-06-18) ##
- Visualisation Version 7: Bug fix pusher visualisation
 
## Features of Release 18c (2021-06-15) ##
- Visualisation Version 6: Buttons for injection and clear of system (contribution by David Berschauer). 

## Features of Release 18b ##
- Bug Fix: consol parameter `-typeB` now recognised.

## Features of Release 18a ##
- Visualisation evaluates simulation instance (A/B) and displays the instance in title. 

## Features of Release 18 ##
- Bug-fix in sticky-flag evaluation

## Features of Release 17 ##
- Visualisation with reduced whitespace and E-Stop.
- Changes in push-button behaviour
- fixes drop issue
- E-Stop in visualisation

## Features of Release 16 ##
- Bug-Fixes in slide movement with feed-separator.
- Move of folder!
- Internal changes for variant management.

## Features of Release 15 ##
- Configurable autostart of simulation on initial write access.

## Maintainer ##
Thomas Lehmann (T.L@haw-hamburg.de)
