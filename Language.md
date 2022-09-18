# The STA language

## Language grammar
```
Call := 'call' Name Expression*
Boolean := 'true' | 'false'

Number := ('+' | '-')? [0-9]+

Literal := Number
            | Boolean

Expression := BinaryOp Expression Expression
            | Literal

ExprToBool := Expression # Uses Symbols From Input and Output and Literals, returns Boolean

FnDef := 'fn' Symbol
            'in' Symbol*
            'out' Symbol*
            'guard' ExprToBool*
            'body' Expression+

Program := (Call | FnDef)*
```
