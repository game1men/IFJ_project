# Basic function

In short node reads from left and writes to right.

id - will be set as unique whe inicialized

## statement list (n_stList)

List of nodes.

Left - any node
Right - n_stList/null

## concatenation (n_concat)
As child node can be any node that returns string value.

## operations (n_add, n_mull, n_div, n_sub, n_comp)

Are executed on values of its childnodes, result is pushed to stack.
As child node can be any node that return value.

if any of operands of any child operation was float set isFloatOperation to true

### comparation (n_comp)

needs to have set cmpType.


## constant (n_constant)

any constant in code

valueInt/valueFloat/valueString - value
vt- variable type
isNull - if null set to true

## variable (n_var)

represents variable,
name - var name
isAssigned - if it is certain that it will be asigned in code set to true
varT - last saved variable type in this variable

## assign variable (n_assignVar)

assigns value from right child to variable
name - var name
right - any node that returns value

## define variable (n_defvar)

defines variable and assigns value from right child to it. (in functions needs to be checked if not already declared in arguments)
name - var name
right - any node that returns value

## write (n_write)

prints left child to stdin.
left - any node that returns value

## read (n_read)
varT - what kind of variable to load
returns read value to stack

## if condition (n_if)

left - condition (n_comp/var/cons)
middle - if body (any node)
middle - else body (any node)

## while (n_while)

left - condition (n_comp/var/cons)
middle - body (any node)

## while (n_while)

left - condition (n_comp/var/cons)
middle - body (any node)

## function (n_funDef)

Declaration and definition of function.

name - name of function
left - n_argLfun

## function argument list (n_argLfun)

list of arguments when declaring function.

name - name of argument
vt - variable type
right - n_argLfun

## call  (n_funCall)

list of arguments when calling function.

name - name of function
left - n_argLcall

## call argument list (n_argLcall)

list of arguments when calling function.

left - any node that returns value
right - n_argLcall

## return (n_return)

right - any node that returns value

| node type   | left node             | middle node | right node    | cmpT | varT | isFloatOperation | isAssigned | isNull | name | valueInt | valueFloat | valueString | isNullable |
|-------------|-----------------------|-------------|---------------|------|------|------------------|------------|--------|------|----------|------------|-------------|------------|
| n_stList    | any                   |             | n_stList/null |      |      |                  |            |        |      |          |            |             |            |
| n_add       | any                   |             | any           |      |      |                  |            |        |      |          |            |             |            |
| n_mull      | any                   |             | any           |      |      |                  |            |        |      |          |            |             |            |
| n_sub       | any                   |             | any           |      |      |                  |            |        |      |          |            |             |            |
| n_div       | any                   |             | any           |      |      |                  |            |        |      |          |            |             |            |
| n_concat    | any                   |             | any           |      |      |                  |            |        |      |          |            |             |            |
| n_comp      | any                   |             | any           | set  |      |                  |            |        |      |          |            |             |            |
| n_constant  |                       |             |               |      | set  |                  |            | set    |      | set      | set        | set         |            |
| n_var       |                       |             |               |      | set  |                  | set        |        | set  |          |            |             | set        |
| n_assignVar |                       |             | any           |      |      |                  |            |        | set  |          |            |             |            |
| n_defvar    |                       |             | any           |      |      |                  |            |        | set  |          |            |             |            |
| n_write     | any                   |             |               |      |      |                  |            |        |      |          |            |             |            |
| n_read      |                       |             |               |      | set  |                  |            |        |      |          |            |             |            |
| n_if        | (n_comp/n_var/n_cons) | any         | any           |      |      |                  |            |        |      |          |            |             |            |
| n_while     | (n_comp/n_var/n_cons) | any         |               |      |      |                  |            |        |      |          |            |             |            |
| n_funDef    | n_argLfun             |             |               |      | set  |                  |            |        | set  |          |            |             | set        |
| n_argLfun   |                       |             | n_argLfun     |      | set  |                  |            |        | set  |          |            |             |            |
| n_funCall   | n_argLcall            |             |               |      | set  |                  |            |        | set  |          |            |             |            |
| n_argLcall  | any                   |             | n_argLcall    |      |      |                  |            |        |      |          |            |             |            |
| n_return    |                       |             | any           |      | set  |                  |            |        |      |          |            |             | set        |