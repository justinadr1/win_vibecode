; client.asm — TCP client connecting to localhost:456 (NASM, Win64)

BITS 64
default rel

extern WSAStartup
extern WSACleanup
extern socket
extern connect
extern recv
extern send
extern closesocket
extern ExitProcess

; -----------------------------
; Structures
; -----------------------------
struc WSADATA
    .wVersion        resw 1
    .wHighVersion    resw 1
    .szDescription   resb 257
    .szSystemStatus  resb 129
    .iMaxSockets     resw 1
    .iMaxUdpDg       resw 1
    .lpVendorInfo    resq 1
endstruc

struc sockaddr_in
    .sin_family resw 1
    .sin_port   resw 1
    .sin_addr   resd 1
    .sin_zero   resq 1
endstruc

; -----------------------------
; Data
; -----------------------------
section .data
wsa:
    istruc WSADATA
    iend

addr:
    istruc sockaddr_in
    iend

msg db "Hello from client", 0
buf times 128 db 0

; -----------------------------
; Code
; -----------------------------
section .text
global start

start:
    ; shadow space + stack alignment
    sub rsp, 28h

    ; WSAStartup(0x202, &wsa)
    lea rdx, [wsa]
    mov ecx, 0202h
    call WSAStartup

    ; socket(AF_INET, SOCK_STREAM, 0)
    mov ecx, 2
    mov edx, 1
    xor r8d, r8d
    call socket
    mov r12, rax        ; SOCKET

    ; sockaddr_in setup
    mov word [addr + sockaddr_in.sin_family], 2
    mov word [addr + sockaddr_in.sin_port], 0C801h    ; htons(456)
    mov dword [addr + sockaddr_in.sin_addr], 0100007Fh ; 127.0.0.1

    ; connect(sock, &addr, 16)
    mov rcx, r12
    lea rdx, [addr]
    mov r8d, 16
    call connect

    ; send(sock, msg, 18, 0)
    mov rcx, r12
    lea rdx, [msg]
    mov r8d, 18
    xor r9d, r9d
    call send

    ; recv(sock, buf, 128, 0)
    mov rcx, r12
    lea rdx, [buf]
    mov r8d, 128
    xor r9d, r9d
    call recv

    ; cleanup
    mov rcx, r12
    call closesocket
    call WSACleanup

    xor ecx, ecx
    call ExitProcess
