# RSA Encrypt/Decrypt Large File use OpenSSL Library

## Background
openssl toolset can not use private key to encrypt large file
## Background of Background
In general, due to performance, RSA only used to exchange symmetric key when begin of communicate.
Regular communicate encrypted by symmetric encryption,  key of symmetric renewed periodic.
But there is nothing is impossible, just depending on what are you willing to pay.
I want to know how slow it is. So I wrote this project.

## Intro
this program able use private/public key to do encrypt/decrypt for large file

## Dependency
openssl library for core function
use many c++11 syntax
cmake for project maintain
use bash script to clean, config, build, install project

## Environment
only has try cross-compile with linaro 4.7 and run on a arm embedded linux

## Usage
{-inkey} filename_of_key
    assign file name of key
{-pubin}
    overwrite key type to public, default is private
{-passphrase | -pass} password
    assign passphrase of key
{-encrypt | -enc}
    this time is do encrypt
{-decrypt | -dec}
    this time is do decrypt
{-in | -infile}
    the file to do encrypt or decrypt
{-out | -outfile}
    after encrypt or decrypt, the file to save result
{-privatekey | -prikey | -pri} filename_of_private_key
    assign file name of key, assign key type to private
    same as "-inkey filename_of_key"
{-publickey | -ppubkey | -pub} filename_of_public_key
    assign file name of key, assign key type to public
    same as "-inkey filename_of_key -pubin"
{-verbose} [level]
    setup debug level, level should be one of 0,1,2,3,4, 0 show nothing
    default debug level is 0 when without -verbose
    default debug level is 4 when -verbose but without level

## Comment of Usage
- -inkey, -pubin, -passphrase, -encrypt, -decrypt, -in, -out is same as
"openssl rsautl"
- this program does not support pipe
- keyform has only try PEM and should be only work for PEM