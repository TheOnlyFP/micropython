## Guide
[https://www.snaums.de/informatik/adding-a-module-to-micropython.html](https://www.snaums.de/informatik/adding-a-module-to-micropython.html)

Change MP_QSTR_myfirstmodule to MP_QSTR_mymodule in example to get a working compule.

### Adding functions to module

Via this forum post: 
[https://forum.micropython.org/viewtopic.php?t=1188](https://forum.micropython.org/viewtopic.php?t=1188)

Follow guide but instead use following syntax:
`{ MP_OBJ_NEW_QSTR(MP_QSTR_hello), (mp_obj_t)&mymodule_hello_obj}`