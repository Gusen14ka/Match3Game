#include "Animation.h"
#include <iostream>

void Animation::update(float dt) {
    if (finished_) return;
    elapsed_ += dt;
    float t = std::min(elapsed_ / duration_, 1.f);
    onUpdate(t);
    if (elapsed_ >= duration_) {
        //std::cout << "Animation finished\n";
        finished_ = true;
        elapsed_ = duration_;
        onComplete();
        if (onCompleteCallback_) {
            onCompleteCallback_();
            //std::cout << "Callback in anim raised\n";
        }
    }
}

bool Animation::isFinished() const { return finished_; }

void Animation::setOnComplete(std::function<void()> callback) {
	onCompleteCallback_ = std::move(callback);
}

void Animation::onComplete() {
    if (onCompleteCallback_) onCompleteCallback_();
}

