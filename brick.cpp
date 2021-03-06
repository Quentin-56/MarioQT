#include "brick.h"
#include "koopa.h"


Brick::Brick()
{
    score = 50;

    currentTexture = QPixmap(loadTexture(":/resources/graphics/blocs/brick.bmp").scaled(BLOCSIZE, BLOCSIZE));
    brickState = NOBRICKSTATE;
    usedBrickTexture = QPixmap(loadTexture(":/resources/graphics/blocs/box-used.bmp").scaled(BLOCSIZE, BLOCSIZE));
}

void Brick::startBlockBounceAnimation()
{
    setPositionY(getHitbox().y());
    if(!timerBounceAnimation.isValid()){
        timerBounceAnimation.start();
    }
    else{
        timerBounceAnimation.restart();
    }
}

void Brick::collisionOnLeftHandler(ObjectModel *o){
    Koopa* koopa = dynamic_cast<Koopa*>(o);
    if(koopa && koopa->isInShell() && koopa->isMoving()){
        if(brickState == NOBRICKSTATE){
            brickState = BREAKBRICK;
        }
        else if(brickState==BRICKWILLGIVECOINONNEXTHIT){
            brickState = GIVECOIN;
            if(!timerSinceCoinBlockHit.isValid()){ timerSinceCoinBlockHit.start(); }
            startBlockBounceAnimation();
        }
    }
}

void Brick::collisionOnRightHandler(ObjectModel *o){
    Koopa* koopa = dynamic_cast<Koopa*>(o);
    if(koopa && koopa->isInShell() && koopa->isMoving()){
        if(brickState == NOBRICKSTATE){
            brickState = BREAKBRICK;
        }
        else if(brickState==BRICKWILLGIVECOINONNEXTHIT){
            brickState = GIVECOIN;
            if(!timerSinceCoinBlockHit.isValid()){ timerSinceCoinBlockHit.start(); }
            startBlockBounceAnimation();
        }
    }
}

void Brick::collisionOnBottomHandler(ObjectModel *o){
    Mario * mario = dynamic_cast<Mario*>(o);
    if(mario){
        bool isMarioBig = mario->isBig();
        if(brickState==USEDCOINBRICK){
            //Do nothing
        }
        else if(isMarioBig && brickState==NOBRICKSTATE){
            brickState = BREAKBRICK; // Will be handled in the gamecontroller later
        }
        else if(brickState==BRICKWILLGIVECOINONNEXTHIT){
            brickState = GIVECOIN; // Will be handled in the gamecontroller later
            if(!timerSinceCoinBlockHit.isValid()){ timerSinceCoinBlockHit.start(); }
            startBlockBounceAnimation();
        }
        else {
            startBlockBounceAnimation();
        }
    }
}

void Brick::animate()
{

    if(timerSinceCoinBlockHit.isValid() && timerSinceCoinBlockHit.elapsed() > timeToGetCoins){
        brickState = USEDCOINBRICK;
        currentTexture = usedBrickTexture;
        timerSinceCoinBlockHit.invalidate();
    }

    if(timerBounceAnimation.isValid() && timerBounceAnimation.elapsed() < animationDuration){
        if(timerBounceAnimation.elapsed() < animationDuration/2){
            setPositionY(position.y()-1.5);
        }
        else{
            setPositionY(position.y()+1.5);
        }
    }
    else{
        setPositionY(getHitbox().y());
    }
}

QList<BrickDebris*> Brick::doBreak(){
    playSound(":/resources/sounds/block-break.wav");
    QList<BrickDebris*> brickDebris;

    BrickDebris * bd1 = new BrickDebris(LEFT);
    bd1->setPositionX(position.x()+BLOCSIZE/4);
    bd1->setPositionY(position.y()+3*BLOCSIZE/4);

    BrickDebris * bd2 = new BrickDebris(UP);
    bd2->setPositionX(position.x()+BLOCSIZE/4);
    bd2->setPositionY(position.y()+BLOCSIZE/4);

    BrickDebris * bd3 = new BrickDebris(RIGHT);
    bd3->setPositionX(position.x()+3*BLOCSIZE/4);
    bd3->setPositionY(position.y()+BLOCSIZE/4);

    BrickDebris * bd4 = new BrickDebris(DOWN);
    bd4->setPositionX(position.x()+3*BLOCSIZE/4);
    bd4->setPositionY(position.y()+3*BLOCSIZE/4);

    brickDebris.append(bd1);
    brickDebris.append(bd2);
    brickDebris.append(bd3);
    brickDebris.append(bd4);

    deletable = true;

    return brickDebris;
}

Coin * Brick::spawnCoin(){
    playSound(":/resources/sounds/coin.wav");
    setBrickState(BRICKWILLGIVECOINONNEXTHIT);
    Coin * coin = new Coin(true);
    coin->moveTo(position.x()+(BLOCSIZE-coin->getCurrentTexture().width())/2, position.y()-BLOCSIZE);
    return coin;
}

void Brick::playSound(QString soundPath){
    QSoundEffect * test = new QSoundEffect();
    test->setSource(QUrl::fromLocalFile(soundPath));
    test->setVolume(0.05);
    test->play();
}
