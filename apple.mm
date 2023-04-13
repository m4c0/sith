@import Foundation;

@interface STHThread : NSThread
@property (nonatomic) void *data;
@property (nonatomic) void (*callback)(void *);
@end
@implementation STHThread
- (void)main {
  self.callback(self.data);
}
@end

extern "C" void *sith_create(void *data, void (*fn)(void *)) {
  STHThread * t = [STHThread new];
  t.callback = fn;
  t.data = data;
  [t start];
  return (__bridge_retained void *)t;
}
extern "C" void sith_destroy(void *nth) {
  STHThread * t = (__bridge_transfer STHThread *)nth;
  while (![t isFinished]) {
    usleep(100);
  }
}
