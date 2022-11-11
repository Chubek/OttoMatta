# OttoMatta

## Proposal

Marriage between a programming language and a database may seem rather coarse. OttoMatta aims to create just such thing.

OttoMatta can be defined as a **Intera-Data-State-Switcher**.

Let's explain the term. Intra-Data implies that data is held within the state machine, but is also decoupled from it. State-Switcher implies that the data are responsible for both the switching of states  and being the subject of the machine.

Data is both the object and the subject of OttoMatta. There's thrtee types of data in OttoMatta. Assertive, Affirmative and Variable. Assertive data are objective data. Affirmative data are descriptive data. Variable data are the data that are acted upon, aka Objective data.

There's several types of Assertive data working in tandem:

1- Enumerative data
2- State transition expressions
4- Action scripts
5- CRON and conditional triggers

You can imagine each `database` in MySQL or MSSQL to be an entire state machine. The various aspects of each machine are held inside separate files in the folder associated with that state machine.

Affirmative data shall be inserted as structures similar to what you see in languages like C and Rust. To create an Affirmative struct:

```
$ smdb -p <password>    #default user is root
$ CREATE AUTOMATA `my_candy_automata` 
$ SELECT AUTOMATA `my_candy_automata`
$ CREATE AFFIRM `my_candy_affirm_struct` -> `{ "id autoint", "candies string", "price int", "ingredients list[2], status_id" }`
$ INSERT INTO `my_candy_affirm_struct` -> `"Mars 2 [Chewy, Syrup, Diabetes] 1", "Bounty 1 [Coconut Walnut] 2"`
```

These are all saved in a file called `~/.smdb/data/my_candy_automata/affirm.otto`. 

Now the Variable data. These data are the data that the state machine changes upon a CRON or conditional trigger. They are stored in various files inside the folder where the aforementioned file (`affirm.otto`) lives. They are named after the field that relegates back to them. They are similar to *foreign keys* in SQL. Except they are decoupled from static or affirmative data. In the case of `my_candy_affirm_struct`, `status_id` serves as the *variable relegator* (aka foreign key). The data for `status_id` is saved inside `status_id.relegate.otto`. To define a variable relegator:

```
$ SELECT AFFIRM `my_candy_affirm_struct`
$ BIND RELEG `status_id` -> `{ "customer_id", "sold bool", "melted bool", "stolen bool", "happened_at datetime" }`
```

You don't get to insert inside variable relegates. You can only modify them via action scripts.

So up until now everything was the old boring SQL with name changes. But now we reach the interesting part. 

Let's go through inserting enums into the DB. These will be saved inside the `assert.otto` file inside the aforementioned folder.

```
$ SELECT AUTOMATA `my_candy_automata`
$ CREATE ASSERT ENUM -> `CandiesArrived, CandySold, CandyStolen, CandyEaten, CandyExpired, CandyMelted`
```

We need to express these states based on how they may preceed and succeed each other. `CandyArrived` must always be the first state. `CandySold` must always be the second --- However, `CandyStolen` can also be the second state. The third state can be `Eaten`, `Melted`, or `Expired`. However, if a Candy is `Expired`, `Eaten` must not happen. If `Melted` takes place, both `Eaten` and `Expired` can happen. 

Your outcomes can be probablistic. You can say `If melted, then maybe eat? I dunno.` You could do the same for expired. But we're not that nasty. Let's just eat it maybe if it's melted.

Expressions must be saved inside the `expr.otto` file. They are separated by a header denoted by `#`. You can have multiple expressions you can call from the 

To express this states in this form:

```
#melted_expired_expr

Init: CandyArrived;
LABEL_1: Follows Init -> CandySold RET -1 | CandyStolen RET -2;
LABEL_2: Follows LABEL_1 -> CandyMelted (30?) RET 0;
LABEL_3: Follows LABEL_1 -> CandyExpired (NOT) RET 1;
LABEL_4: Follows LABEL_2, LABEL_3 -> CandyEaten RET 2;
```

In this expression:

1- `CandyArrived`, the initial state. Initial state msut be marked as `Init`.
2- `CandyStolen` and `CandySold` can both be the second state, labeled by `LABEL_1`.
3- If `CandyExpired`, `(NOT)` denotes that `CandyEaten` must not happen. But if candy melted, `(30?)` denotes that it only has 30% chance of being eaten.

The `RET` statements is to be used with actions. Your can return a `RET <number>` from any of the lines.

Now the action scripts. They are basic commands that operate on variable relegates based on affirmative data. 

You need to write your scripts inside a single file called `actions.otto`. You need to separate each action with the same header you used for the corresponding expression.

For example to define an action for the above expression:

```
#melted_expired_expr
- Struct: `my_candy_affirm_struct`;
- Relegate: status_id;
- Trigger: customer_trigger

CASY ANY:
UPSERT `customer_id` -> $cust_id

CASE -1:
UPSERT `sold` -> `true`;

CASE -2:
UPSERT `stolen` -> `true`;

CASE 0:
UPSERT `melted` -> `true`;

CASE 1:
UPSERT `expired` -> `true`;

CASE 2:
UPSERT `eaten` -> `true`;
```

But how do we trigger these events? A `Trigger` can contain datafields which can be accessed by putting `$` behind the names assigned to them in the trigger. And a trigger can happen in three ways:

1- Manual: using the commandline
2- Conditional: using a Python script
3- CRON: Based on time, once or on a regular basis

To incite an event manually you need to 