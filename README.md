# SAT Solver

## Preprocessing
- Read input
    - From GUI (gtkmm)
    - From file
    - From command line argument
- Clean up input
- Parse formula
- Replace equivalence and implication
- Apply negations (De Morgan)
- Apply distributivity
- Convert formula to CNF
- Convert to sets
- Simplify formula
    - Remove clauses that are always true

## Solving
### Recursive
- Choose an `l` literal
- Set value
- Remove `l` from clauses where it is 0
- Remove clauses where `l` is 1
- Repeat for each literal
- The formula is satisfiable if it becomes empty

### DPLL

## Displaying
- Display each version of the formula
- Show a table with literal values
- Allow exporting to csv
