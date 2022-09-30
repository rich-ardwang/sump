#pragma once
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class IThreadCallback {
public:
	virtual void OnWork(void) = 0;
};

class WorkThread {
public:
	WorkThread(IThreadCallback* thread)
		: thread_cb_(thread) {
	}

	void Start(void) {
		worker_ = std::make_shared<std::thread>(std::bind(&WorkThread::work, this));
		std::unique_lock<std::mutex> lock_guard(mutex_);
		is_start_ = true;
		cond_var_.notify_all();
	}

	void Stop(void) {
		std::unique_lock<std::mutex> lock_guard(mutex_);
		is_start_ = false;
		cond_var_.notify_all();
		worker_->join();
	}

private:
	void work(void) {
		std::unique_lock<std::mutex> lock_guard(mutex_);
		cond_var_.wait(lock_guard, [this]() { return is_start_; });

		if (thread_cb_ != nullptr) {
			thread_cb_->OnWork();
		}
	}

private:
	bool is_start_ = false;
	IThreadCallback* thread_cb_ = nullptr;
	std::mutex mutex_;
	std::condition_variable cond_var_;
	std::shared_ptr<std::thread> worker_;
};