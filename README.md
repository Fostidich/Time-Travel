# Time-Travel

## OVERVIEW
Bring order to your folders by extracting dates from messy filenames.
Files in the specified path are scanned, and if a date is detected in their names, the file is renamed with YYYY-MM-DD format.

## NOTES
- American formatted dates are unrecognised;
- years are valid if between 2000 and 2099;
- if a filename has 2 dates, only the first one is considered;
- path can be either absolute or relative;
- folders are immune to renaming;
- searches are made by sections; if a section finds a date, the other sections are skipped. Sections are:
    - dates with month in literal format (along with both year and day);
    - dates with month in literal format (along with just the day);
    - dates made of three numbers (year, month, day);
    - dates made of two numbers (no year).

## USAGE

```bash
time-travel {{path}}
```

###### Code written by Francesco Ostidich
