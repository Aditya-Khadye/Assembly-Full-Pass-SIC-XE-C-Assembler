COPY    START   1000
FIRST   LDA     ALPHA
        STA     BETA
        LDCH    CHARZ
        STCH    CHARZ
        BYTE    C'EOF'
        WORD    3
        RESW    1
        RESB    2
ALPHA   WORD    5
BETA    RESW    1
CHARZ   BYTE    C'Z'
        END     FIRST