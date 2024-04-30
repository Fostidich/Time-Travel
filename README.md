# Time-Travel

## OVERVIEW
Bring order to your folders by extracting dates from messy filenames.

Files in the specified path are scanned, and if a date is detected in their names, the file is renamed with YYYY-MM-DD format.

## NOTES
- American formatted dates are unrecognised;
- years are valid if between 2000 and 2099;
- if a filename has 2 dates, only the first one is considered;
- path can be either absolute or relative;
- folders are immune to renaming.

## USAGE

```bash
time-travel {{path}}
```

###### Code written by Francesco Ostidich
