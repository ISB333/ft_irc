# JOIN check-list:

---

## Basic syntax:

- [ ] **Cannot** join channel (461):

> \<JOIN\>

> \<JOIN\> \<,\>

---

## Channel not yet created:

> \<JOIN\> \<#main1\>

- [ ] Broadcast to none.
- [ ] Get **no** topic.
- [ ] Get (none) names (353).
- [ ] Get ENDOFNAMES (366).
- [ ] Is the main operator.

>> \<TOPIC\> \<#main1\> \<crusade\>

---

## Channel already created:

> \<JOIN\> \<#main1\>

~~(No check list on this one)~~

> \<JOIN\> \<#mAiN1,#MaiN1\>

- [ ] Broadcast to the first two.
- [ ] Get **crusade** topic.
- [ ] Get (none) names (353).
- [ ] Get ENDOFNAMES (366).
- [ ] Is **not** operator.
- [ ] **Declines** second time (443).
- [ ] Should be **normalized into lowercase** and **same target**.

---

## Invalid syntax

- [ ] Should always **denied** and return **476** errors:

> \<JOIN\> \<main1\>

> \<JOIN\> \<#ma🎩in1\>

> \<JOIN\> \<#maiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiin\>

---

## Multiple channels in once

- [ ] Should **work** with all channels:

> \<JOIN\> \<#main2,#main3\>
> \<JOIN\> \< ,#main4,#main5, \>

- [ ] Should **only work** with *main4 and main1*;
- [ ] *main1* should **return** *crusade topic*:

> \<JOIN\> \<#main6,main7,#main1 #main8 main9 #main10\>

- [ ] Should **only work** with *main11*;
- [ ] *main12* should be considered as a **key**;

> \<JOIN\> \<#main11 , #main12\>

---

## MAXI-JOIN!

- [ ] Server must **survive**:

> \<JOIN\> \<#1,#2,#3,#4,#5,#6,#7,#8,#9,#10,#11,#12,#13,#14,#15,#16,#17,#18,#19,#20,#21,#22,#23,#24,#25,#26,#27,#28,#29,#30,#31,#32,#33,#34,#35,#36,#37,#38,#39,#40,#41,#42,#43,#44,#45,#46,#47,#48,#49,#50,#51,#52,#53,#54,#55,#56,#57,#58,#59,#60,#61,#62,#63,#64,#65,#66,#67,#68,#69,#70,#71,#72,#73,#74,#75,#76,#77,#78,#79,#80,#81,#82,#83,#84,#85,#86,#87,#88,#89,#90,#91,#92,#93,#94,#95,#96,#97,#98,#99,#100\>

---

## Mode:

### Free-for-all:

>> \<JOIN\> \<#key,#invite,#limit\>

#### Key:

> \<MODE\> \<#key +k secret\>

- [ ] *#main1* should **always be accessible** when specified!

- [ ] *#key* is **accessible**:

> \<JOIN\> \<#key secret\>

> \<JOIN\> \<#main1,#key key1,secret\>

> \<JOIN\> \<#main1,#key key1,secret,key3\>

> \<JOIN\> \<#main1,#key secret,secret\>

- [ ] *#key* is **denied** (code 475):

> \<JOIN\> \<#key\>

> \<JOIN\> \<#key key1,secret\>

> \<JOIN\> \<#main1,#key key1,key2,key3\>

> \<JOIN\> \<#main1,#key secret\>

> \<JOIN\> \<#key sec,ret\>

> \<JOIN\> \<#key key1 secret\>

---

#### Invite:

> \<MODE\> \<#invite +i\>

> \<JOIN\> \<#invite\>

- [ ] *#invite* is **denied**.

> \<INVITE\> \<@you\> \<#invite\>

> \<JOIN\> \<#invite\>

- [ ] *#invite* is **accessible**.

---

#### Limit:

> \<MODE\> \<#limit +l 2\>

> \<JOIN\> \<#limit\>

- [ ] *#limit* is **accessible** for the second client.

> \<JOIN\> \<#limit\>

- [ ] *#limit* is **denied** for the third client.

//TODO: Add KICK test when available here!

---

### Team up:

>> \<JOIN\> \<#modes\>

>> \<MODE\> \<#key +ikl secret 2\>

- [ ] *#modes* should always be **denied** until operator invite you:

> \<JOIN\> \<#modes\>

> \<JOIN\> \<#modes secret\>

>> \<INVITE\> \<@you\> \<#invite\>

> \<JOIN\> \<#modes\>

> \<JOIN\> \<#modes key\>

- [ ] *#modes* should now be **accessible**:

> \<JOIN\> \<#modes secret\>

- [ ] *#modes* should be no more **accessible** for the next client:

> \<JOIN\> \<#modes\>

> \<JOIN\> \<#modes secret\>

>> \<INVITE\> \<@you\> \<#invite\>

> \<JOIN\> \<#modes secret\>