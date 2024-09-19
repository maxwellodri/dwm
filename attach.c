static const int attachdirection = 4;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
void choose_attach(Client* c) {
    int chosen = attachdirection;
    if (c->attachdirection != -1) {
        chosen=c->attachdirection;
    }
    switch(chosen){
        case 1:
            attachabove(c);
            break;
        case 2:
            attachaside(c);
            break;
        case 3:
            attachbelow(c);
            break;
        case 4:
            attachbottom(c);
            break;
        case 5:
            attachtop(c);
            break;
        default:
            attach(c);
    }

}
